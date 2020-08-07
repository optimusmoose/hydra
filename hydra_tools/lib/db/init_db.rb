# frozen_string_literal: true

require 'sqlite3'

module Db
  # Initialization class for Sqlite DB
  class Init
    attr_reader :dir, :db, :empty
    def initialize(dir)
      @dir = dir
      @empty = true
    end

    def perform
      # Remove this line persistent databases are desired
      delete_existing_db

      if File.exist? "#{@dir}.ms.db"
        @db = SQLite3::Database.open "#{@dir}.ms.db"
        @empty = false
      else
        create_db
        build_tables
      end
      self
    end

    def create_index
      @db.execute <<-SQL
      CREATE INDEX IF NOT EXISTS mz_rt ON ms1_points (mz,rt)
      SQL
    end

    private

    def delete_existing_db
      return unless File.exist? "#{@dir}.ms.db"

      File.delete("#{@dir}.ms.db")
    end

    def create_db
      @db = SQLite3::Database.new "#{@dir}.ms.db"
      @db.execute('PRAGMA journal_mode = OFF')
      @db.execute('PRAGMA synchronous=OFF')
    end

    def build_tables
      @db.execute <<-SQL
      create table ms1_points (
        mz REAL,
        intensity REAL,
        rt REAL
      )
      SQL
    end
  end
end
