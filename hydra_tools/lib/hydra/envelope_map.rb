# frozen_string_literal: true

# Takes a list of features and builds a envelope_map.json
# if features don't have summed intensities, query db

module Hydra
  # Object for envelope_map json
  class EnvelopeMap
    def initialize(features)
      @features = reject_features(features)
    end

    def perform
      h = { "envelopes": @features }
      # puts h
      @json = JSON.pretty_generate h
      self
    end

    def write(f)
      f = File.open(f, 'w')
      f.write @json
      f.close
    end

    private

    def reject_features(features)
      # Discard features that have a CS of 0
      rejects = []
      features.each do |feature|
        rejects << feature.id if feature.total_abundance.zero?
      end

      unless rejects.empty?
        puts "#{rejects.size} features rejected because no points are within convex hull of traces"
        puts rejects
      end

      features.reject { |f| f.charge_state.zero? || f.total_abundance.zero? }.map(&:to_envelope_map)
    end
  end
end
