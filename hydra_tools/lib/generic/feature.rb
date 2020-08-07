# frozen_string_literal: true

module Generic
  # Generic feature class for use as feature or envelope
  class Feature
    attr_accessor :hulls, :id, :monoisotopic_mz, :monoisotopic_rt, :charge_state, :min_mz, :max_mz, :min_rt, :max_rt, :total_abundance, :isotope_ratios
    def initialize
      @matches = []
      @hulls = []
    end

    def from_csv(h)
      @id = "f_#{rand(0..100_000_000_000_000_000)}"
      @monoisotopic_mz = h['monoIsotopicMz'].to_f
      @monoisotopic_rt = h['monoIsotopicRT'].to_f
      @charge_state = h['chargeState'].to_i
      @min_mz = h['minMZ'].to_f
      @max_mz = h['maxMZ'].to_f
      @min_rt = h['minRT'].to_f
      @max_rt = h['maxRT'].to_f
      @total_abundance = h['totalAbundance'].to_f
      @isotope_ratios = h['relativeIntensities'].split('-').map(&:to_f)
      self
    end

    def to_envelope_map
      {
        "id": @id,
        "matches": @matches,
        "envelope": 
        {
          "monoisotopicMZ": @monoisotopic_mz,
          "monoisotopicRT": @monoisotopic_rt,
          'minRT': @min_rt,
          'maxRT': @max_rt,
          'minMZ': @min_mz,
          'maxMZ': @max_mz,
          "chargeState": @charge_state,
          "totalAbundance": @total_abundance,
          "isotopicRatios": @isotope_ratios
        }
      }
    end

    def set_maxima_minima
      # x = RT
      # y = MZ
      @hulls.each(&:calc_bounds)

      @max_mz = @hulls.map(&:max_mz).max
      @min_mz = @hulls.map(&:min_mz).min
      @max_rt = @hulls.map(&:max_rt).max
      @min_rt = @hulls.map(&:min_rt).min
      self
    end

    def calc_summed_intensities
      if @hulls.map(&:summed_intensity).include?(-1)
        raise 'negative summed intensity of -1'
      end

      @total_abundance = @hulls.map(&:summed_intensity)
                               .sum
      
      return if @total_abundance == 0.0 # Guard clause if total_abundance == 0.0, indicative of all traces with 0 intensity
      @isotope_ratios = @hulls.map { |hull| hull.summed_intensity / @total_abundance }
    end
  end

  # A trace, generic for use with OpenMS, or JSMS
  class ConvexHull
    attr_accessor :id, :envelope_index, :x_vals, :y_vals, :min_mz, :max_mz, :min_rt, :max_rt, :pos, :summed_intensity
    def initialize
      @x_vals = []
      @y_vals = []
      @summed_intensity = -1
    end

    def calc_bounds
      @x_vals.map!(&:to_f).sort!
      @y_vals.map!(&:to_f).sort!
      @min_mz, *, @max_mz = *@y_vals
      @min_rt, *, @max_rt = *@x_vals
    end

    def from_csv(line)
      @id = line['traceID'].to_i
      @envelope_index = line['envelopeID'].to_i - 1
      @min_mz = line['minMZ'].to_f
      @max_mz = line['maxMZ'].to_f
      @min_rt = line['minRT'].to_f
      @max_rt = line['maxRT'].to_f
      @summed_intensity = line['intensitySum'].to_f
      self
    end

    def as_convex_hull(nr)
      # return ox convex hull node
      convex_hull = Ox::Element.new('convexhull')
      convex_hull[:nr] = nr.to_s
      pt1 = Ox::Element.new('pt')
      pt1[:x] = @min_rt.to_s
      pt1[:y] = @min_mz.to_s
      pt2 = Ox::Element.new('pt')
      pt2[:x] = @max_rt.to_s
      pt2[:y] = @min_mz.to_s
      pt3 = Ox::Element.new('pt')
      pt3[:x] = @max_rt.to_s
      pt3[:y] = @max_mz.to_s
      pt4 = Ox::Element.new('pt')
      pt4[:x] = @min_rt.to_s
      pt4[:y] = @max_mz.to_s
      convex_hull << pt1
      convex_hull << pt2
      convex_hull << pt3
      convex_hull << pt4
      convex_hull
    end
  end
end
