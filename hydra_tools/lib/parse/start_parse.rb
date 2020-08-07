# frozen_string_literal: true

require_relative '../JSMS/parsers/csv_to_envelope_map'
require 'db/init_db'
require 'mzXML/parse_ms1'
require 'openMS/parse_featureXML'
require 'openMS/fill_traces'
require 'hydra/envelope_map'

module Parse
  # Runner for parse command
  class StartParse
    def initialize(dir)
      @dir = dir + '/'
    end
    def perform
      find_files
    end
    private
    def find_files
      puts "Using directory #{@dir}"

      files = Dir["#{@dir}*"]

      csvs = files.select { |f| f.match? /.csv$/ }
      featureXMLs = files.select { |f| f.match? /.featureXML$/ }
      mzXMLs = files.select { |f| f.match? /.mzXML$/}

      if csvs.size == 2

        puts "Using CSV as filetype, JSMS input"
        JSMS::Parsers::CSVToEnvelopeMap.new(@dir).perform

      elsif featureXMLs.size == 1 && mzXMLs.size == 1

        puts "Using featureXML as filetype, OpenMS input"
        mzXML = files.select { |f| f.match? /.mzXML$/ }
        if mzXML.size > 1
          raise "Multiple mzXML files"
        elsif mzXML.size == 0
          raise "No mzXML files"
        end

        mzXML = mzXMLs.first
        featureXML = featureXMLs.first

        # Initialize db
        puts "Building database"
        db_initializer = Db::Init.new(@dir)
                                 .perform
        puts "Finished building database"

        db = db_initializer.db

        # Parse ms1 points in sqlite, return db filename
        puts "Parsing .mzXML"
        mzXML_parser = MzXML::ParseMS1.new(mzXML, db).perform if db_initializer.empty
        puts "Finished parsing .mzXML\n"

        # Add indices in db
        puts "Adding indices"
        db_initializer.create_index
        puts "Finished adding indices\n"

        # Parse features from featureXML
        puts "Parsing .featureXML"
        features = OpenMS::ParseFeatureXML.new(featureXML)
                                          .perform
                                          .features
        puts "Finished parsing .featureXML"

        # Fill in traces from sqlite
        puts "Calculating isotope ratios for traces"
        OpenMS::FillTraces.new(db, features)
                          .perform
        puts "Finished calculations"
        
        # Build envelope_map
        puts "Building envelope_map.json"
        Hydra::EnvelopeMap.new(features)
                          .perform
                          .write("#{@dir}envelope_map.json")
        puts "Finished building map"
      else
        puts "No files found"

        unless csvs.empty?
          puts "found"
          puts csvs
        end
        unless featureXMLs.empty?
          puts "found"
          puts featureXMLs
        end
        unless mzXMLs.empty?
          puts "found"
          puts mzXMLs
        end
      end
    end
  end
end
