# frozen_string_literal: true


require 'csv'
require 'json'

require 'generic/feature'
require 'openMS/featureXML'

module JSMS
  module Parsers
    # Handler for converer JSMS .csv to envelope_map
    class CSVToEnvelopeMap
      attr_accessor :file, :features
      def initialize(dir)
        @dir = dir
        @outfile = @dir + 'envelope_map.json'
        @featureXML = @dir + 'envelopes.featureXML'
        @features = []
        @traces = []
      end

      def perform
        if find_files
          parse
          map_traces_to_envelopes
          write
        else
          puts "Did not find a file matching regex /envelope/"\
          " and/or /trace/"
        end
        self
      end

      private

      def find_files
        files = Dir["#{@dir}*.csv"]
        @traces_f = files.select { |f| f.match?(/traces/) }.first
        @envelopes_f = files.select { |f| f.match?(/envelope/) }.first

        if @traces_f.nil? || @envelopes_f.nil?
          return false
        else
          return true
        end
      end

      def map_traces_to_envelopes
        @traces.each do |trace|
          @features[trace.envelope_index].hulls << trace
        end
      end

      def parse

        # Parse envelopes
        f = File.open(@envelopes_f)
        CSV.foreach(f, headers: true) do |line|
          @features << Generic::Feature.new.from_csv(line)
        end
        f.close

        # Parse traces
        f = File.open(@traces_f)
        CSV.foreach(f, headers: true) do |line|
          @traces << Generic::ConvexHull.new.from_csv(line)
        end

        puts "traces: #{@traces.size}"
        puts "features: #{@features.size}"
      end

      def write
        # Write out JSON
        outfile = File.open(@outfile, 'w')
        features = @features.reject { |f| f.charge_state.zero?}
        json = {"envelopes": features.map(&:to_envelope_map)}
        outfile.write JSON.pretty_generate(json)
        outfile.close

        # Write out featureXML
        OpenMS::FeatureXML.new(@featureXML, features).perform
                                                     .write_featureXML

        self
      end
    end
  end
end
