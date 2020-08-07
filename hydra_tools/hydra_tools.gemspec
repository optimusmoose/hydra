# frozen_string_literal: true

Gem::Specification.new do |s|
  s.name        = 'hydra_tools'
  s.version     = '0.5.0'
  s.date        = '2010-04-28'
  s.summary     = 'Hydra Tools'
  s.description = 'A Ruby tool for preparing files for Hydra'
  s.authors     = ['Michael Callahan']
  s.email       = 'michaelncallahan@gmail.com'
  s.files       = [
    'lib/hydra_tools.rb',
    'lib/JSMS/parsers/csv_to_envelope_map.rb',
    'lib/parse/start_parse.rb',
    'lib/generic/feature.rb',
    'lib/openMS/parsers/featureXML.rb',
    'lib/openMS/parse_featureXML.rb',
    'lib/openMS/fill_traces.rb',
    'lib/openMS/featureXML.rb',
    'lib/mzXML/parsers/ms1_points.rb',
    'lib/hydra/envelope_map.rb',
    'lib/db/init_db.rb',
    'lib/mzXML/parse_ms1.rb'
  ]
  s.license = 'MIT'
  s.homepage = 'https://www.primelabs.ms'
  s.executables << 'hydra_tools'
  s.add_dependency('ox', '~> 2.4')
  s.add_dependency('sqlite3', '~> 1.3')
  s.add_dependency('thor', '~> 1.0')
end
