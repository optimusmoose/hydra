# frozen_string_literal: true

require 'generic/feature'

module OpenMS
  module Parsers
    # Parses a featureXML, returns a list of features
    class FeatureXML < ::Ox::Sax
      attr_accessor :features, :count
      def initialize(features)
        @features = features
        @count = 0
      end
      def get_features
        return @features
      end
      def start_element(name)
        # puts "start: #{name}"
        if name == :feature
          @count += 1
          @feature = Generic::Feature.new
        elsif name == :convexhull
          @hull = Generic::ConvexHull.new
        elsif name == :charge
          @on_charge = true
        elsif name == :position
          @on_position = true
        end
      end
      def end_element(name)
        # puts "end: #{name}"
        if name == :feature
          @features << @feature
          @pos = -1
        elsif name == :convexhull
          @feature.hulls << @hull
        elsif name == :charge
          @on_charge = false
        elsif name == :position
          @on_position = false
        end
      end
      def attr(name, value)
        # puts "  #{name} => #{value}"
        if @feature
          case name
          when :x
            @hull.x_vals << value
          when :y
            @hull.y_vals << value
          when :nr
            @hull.pos = value.to_i
          when :id
            # @feature.id = value.split('_').last
            @feature.id = value
          when :dim
            @pos = value.to_i
          end
        end
      end
      def text(value)
        # puts "text #{value}"
        if @on_charge
          @feature.charge_state = value.to_i
        elsif @pos == 0 && @on_position
          @feature.monoisotopic_rt = value.to_f
        elsif @pos == 1 && @on_position
          @feature.monoisotopic_mz = value.to_f
        end
      end
    end
  end
end