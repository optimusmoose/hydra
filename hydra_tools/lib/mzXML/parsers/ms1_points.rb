# frozen_string_literal: true

require 'ox'
require 'base64'
require 'zlib'

module MzXML
  module Parsers
    class MS1Points < Ox::Sax
      def initialize(db)
        @db = db
        @id = 0
        @on_scan = false
        @on_ms1 = false
        @on_peaks = false
        @rt = -1
      end
    
      def start_element(name)
        # puts "start: #{name}"
        @on_peaks = true if name == :peaks
      end
      def end_element(name)
        # puts "end: #{name}";
        if name == :peaks
          @on_peaks = false if name == :peaks
        end
      end
      def attr(name, value)
        # puts "  #{name} => #{value}";
        if name == :msLevel && value == '1'
          @on_ms1 = true
        elsif name == :msLevel && value != '1'
          @on_ms1 = false
        end
        if @on_ms1 && name == :retentionTime
          @rt = calc_rt(value)
        end

        @compression_type = value if name == :compressionType # Set Compression Type
        @precision = value if name == :precision # Set Precision

      end
      def text(value)
        # puts "text #{value}";
        if @on_ms1 && @on_peaks

          peaks = Base64.decode64(value)
          z_inf = Zlib::Inflate.new(window_bits = 32)
          peaks = z_inf.inflate(peaks) if @compression_type == "zlib"
          peaks = peaks.unpack(@precision.to_i == 32 ? 'g*' : 'G*').each_slice(2).to_a
          
          sql = <<-SQL
          INSERT INTO ms1_points (mz, intensity, rt) VALUES
          SQL

          vals = []
          peaks.each do |peak|
            vals << "(#{peak[0]}, #{peak[1]}, #{@rt})"
          end

          sql = sql + vals.join(',')

          @db.transaction
          @db.execute(sql)
          @db.commit
        end
      end

      private

      def calc_rt(rt)
        
        seconds = rt.match? /S/
        
        rt = rt.gsub(/[A-Za-z]+/,'')
                .to_f
                
        unless seconds
          rt *= 60
        end
        rt
      end
    end
  end
end