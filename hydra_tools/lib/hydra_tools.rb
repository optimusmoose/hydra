# frozen_string_literal: true

require 'thor'

require_relative 'parse/start_parse'

# CLI class
class HydraTools < Thor
  desc 'parse', 'parse a file'
  def parse(f)
    Parse::StartParse.new(f).perform
  end
end
