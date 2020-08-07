# frozen_string_literal: true

require 'ox'
require 'openMS/parsers/featureXML'

module OpenMS
  # Handler for parsing featureXML
  class ParseFeatureXML
    attr_reader :features
    def initialize(featureXML)
      @featureXML = featureXML
      @features = []
    end
    def perform
      handler = OpenMS::Parsers::FeatureXML.new(@features)
      f = File.open(@featureXML)
      Ox.sax_parse(handler, f)
      self
    end
  end
end