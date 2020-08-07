# frozen_string_literal: true

require 'db/init_db'
require 'mzXML/parsers/ms1_points'

module MzXML
  # handler class for parsing and inserting ms1 points
  class ParseMS1
    attr_reader :mzXML, :db
    def initialize(mzXML, db)
      @db = db
      @mzXML = mzXML
    end
    def perform
      handler = MzXML::Parsers::MS1Points.new(@db)
      f = File.open(@mzXML)
      Ox.sax_parse(handler, f)
    end
  end
end
