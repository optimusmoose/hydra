# frozen_string_literal: true

module OpenMS
  # Fills traces with summed intensity
  class FillTraces
    def initialize(db, features)
      @db = db
      @features = features
    end
    def perform
      fill
      self
    end
    private
    def fill
      @features.each do |feature|
        feature.set_maxima_minima
        feature.hulls.each do |hull|
          sql = <<-SQL
          SELECT sum(intensity) FROM ms1_points
          WHERE mz >= #{hull.min_mz}
            AND mz <= #{hull.max_mz}
            AND rt >= #{hull.min_rt}
            AND rt <= #{hull.max_rt}
          SQL

          results = @db.execute sql
          results.flatten!

          if results.include? nil
            # puts "Feature: #{feature.id}"\
            # " Trace: #{hull.pos}"\
            # " has no points within bounds of trace, using 0.0"
            hull.summed_intensity = 0.0
          else
            hull.summed_intensity = results.first
                                           .to_f
          end
        end
        feature.calc_summed_intensities
      end
    end
  end
end
