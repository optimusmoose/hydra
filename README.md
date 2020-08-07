# Hydra

Hydra is a fast MS1 direct matching algorithm for run-to-run feature correspondence.

## Requirements
- CMake >= 3.1
- Linux
  - GNU Compiler
    - GCC >= 8
- Mac
  - Apple Clang >= 11
- Ruby >= 2.6.3 - for usage of Hydra Tools

## Build
Included in the Hydra directory is a CMake file.
Perform an out of source build in another directory

```
$ cd /path/to/hydra
$ mkdir build
$ cd build
$ cmake ../hydra
$ make
$ make install
```

## Usage

```$ ./Hydra outfile ~/path/outfile file ~/path/A/file.json file ~/path/B/file.json mz_distance .1 rt_distance 25 cs_enforced 1```

### Parameters

- mz_distance
  - The maximum allowable distance in m/z for two features to be compared.
  - Default: 1, ```float```, interval (0,infinity).
    - Example:
    - ```$ Hydra mz_distance .1 ```
- rt_distance
  - The maximum allowable distance in RT for two features to be compared.
  - Default: 25, ```float```, interval (0,infinity).
    - Example:
    - ```$ Hydra rt_distance 150```
- cs_enforced
  - Charge state equality for feature comparison, and cluster construction is enforced.
  - Default: 1, ```int```, interval [0,1].
  - Example:
    - ```$ Hydra cs_enforced 1 ```
    - ```$ Hydra cs_enforced 0 ```
- outfile
  - File for the correspondence output to be written to. ```.consensusXML``` will be appended to the end.
  - Default: none, ```string```.
  - Example:
    - ```$ Hydra outfile ~/path/to/file/outfile```

- file
  - Input file. Multiple files need to include a file flag for each.
  - Requirement: Number of files > 1.
  - Default: none, ```string```.
  - Example:
    - ```$ Hydra file ~/path/A/file.json file ~/path/B/file.json```

- help or h
  - Prints out parameter usage assistance.
  - Example:
  - ```$ Hydra h```

Hydra takes a JSON input defining features:

```
{
  "envelopes" :
  [
    {
      "id": "0",
      "envelope": {
        "monoisotopicMZ": 500.0,
        "monoisotopicRT": 3000.0,
        "chargeState": 2,
        "isotopicRatios": [
          0.30,
          0.40,
          0.30
        ]
      }
    },
    ...,
    {
      "id": "5000",
      "envelope": {
        "monoisotopicMZ": 600.0,
        "monoisotopicRT": 4000.0,
        "chargeState": 2,
        "isotopicRatios": [
          0.20,
          0.50,
          0.29,
          0.01
        ]
      }
    }

  ]
}
```

## Hydra Tools

To convert to Hydra's JSON format Hydra Tools has been provided. Hydra Tools is a Ruby Gem that supports files from JSMS and OpenMS. We encourage the use of [RVM](https://rvm.io/) to faciliate Ruby installation and usage.

## Installing Gem

Install Ruby version >= 2.6.3.
```
$ cd ~/path/to/gem
$ gem build hydra_tools.gemspec
$ gem install hydra_tools-0.5.0.gem
```
Open a new shell session.


### JSMS Support

[JSMS](https://github.com/optimusmoose/jsms) allows for isotope tracing, and trace clustering into features. CSVs with envelopes and traces can be exported. Hydra Tools can be used to convert these CSVs into an envelope_map.json for Hydra. Additionaly, a ```.featureXML``` file be written so the features may be inspected in OpenMS.

```
$ cd ~/path/to/CSVs/
$ ls
envelopes.csv
traces.csv
$ hydra_tools parse .
...
$ cd
$ hydra_tools parse ~/path/to/CSVs/
```

### OpenMS Support

[OpenMS](https://www.openms.de/) is a powerful tool for LC/MS data processing and analysis. Using a feature detection algorithm, process LC/MS data files and export the runs as ```.featureXML```. Feature lists exported as ```.featureXML``` that contain features, and convex hulls with bounds are supported with Hydra Tools. The convex hulls don't come annoted with intensity information. Therefore, in order to use ```.featureXML```, a ```.mzXML``` file must be included. The ```.featureXML``` will be parsed and the convex hulls will be converted to traces, then the ```.mzXML``` file will be parsed. The traces will be populated with points, so the isotopic ratios can be calculated. The result is a ```envelope_map.json``` file in the directory.

```
$ cd ~/path/to/files
$ ls
run.featureXML
run.mzXML
$ hydra_tools parse .
```


## Output

Hydra outputs a file as specified by the user with ```.consensusXML``` appended to the end. This allows the user to manually validate/inspect the correspondence with OpenMS Top View.



