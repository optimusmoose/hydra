# This would be used to take a list of features and build a featureXML
# Use case is currently to take a JSMS input CSV and print out a envelope_map
# featureXML for inspecting.

require 'ox'

module OpenMS
  # Takes features, writes featureXML
  class FeatureXML
    attr_accessor :features, :filename
    def initialize(xml_outfile, features)
      @features = features
      @doc = Ox::Document.new
      @xml_outfile = xml_outfile
    end
    def perform
      build_header
      fm = build_feature_map
      build_user_param(fm)
      fl = build_feature_list(fm)
      @features.each do |feature|
        # p feature
        feature_node = Ox::Element.new('feature')
        feature_node[:id] = feature.id
        feature_node << position_node(0, feature.monoisotopic_rt)
        feature_node << position_node(1, feature.monoisotopic_mz)
        charge_node = Ox::Element.new('charge')
        intensity_node = Ox::Element.new('intensity')
        intensity_node << feature.total_abundance.to_s
        charge_node << feature.charge_state.to_s
        feature_node << intensity_node
        feature_node << charge_node
        # insert traces here
        add_hulls(feature, feature_node)
        # p feature.traces.size
        fl << feature_node
      end
      self
    end
    def write_featureXML
      # puts @db
      puts @xml_outfile
      f = File.open(@xml_outfile, 'w')
      f.write(Ox.dump(@doc))
      f.close
      self
    end
    private
    def add_hulls(feature, feature_node)
      feature.hulls.each_with_index do |trace, i|
        feature_node << trace.as_convex_hull(i)
      end
    end
    def position_node(dim, val)
      pos = Ox::Element.new('position')
      pos[:dim] = dim
      pos << val.to_s
      pos
    end
    def build_header
      instruct = Ox::Instruct.new(:xml)
      instruct[:version] = '1.0'
      instruct[:encoding] = 'ISO-8859-1'
      @doc << instruct
    end
    def build_feature_map
      fm = Ox::Element.new('featureMap')
      fm[:version] = '1.9'
      fm[:id] = 'fm_' + (rand(0..1_000_000_000_000)).to_s# Assign featureMap id to random large int
      fm['xsi:noNamespaceSchemaLocation'] = 'https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/FeatureXML_1_9.xsd'
      fm['xmlns:xsi'] = 'http://www.w3.org/2001/XMLSchema-instance"'
      @doc << fm
      fm
    end
    def build_user_param(fm)
      up = Ox::Element.new('UserParam')
      up[:type] = 'stringList'
      up[:name] = 'spectra_data'
      up[:value] = "[#{@mz_file}]"
      fm << up
    end
    def build_feature_list(fm)
      fl = Ox::Element.new('featureList')
      fl[:count] = @features.size
                            .to_s
      fm << fl
      fl
    end
  end
end