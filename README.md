# Racon
[![Published in Genome Research](https://img.shields.io/badge/published%20in-Genome%20Research-blue.svg)](https://doi.org/10.1101/gr.214270.116)

Consensus module for raw de novo DNA assembly of long uncorrected reads. 
**Forked from main repo for independent modification & use of liftover branch**

## Description
Racon is intended as a standalone consensus module to correct raw contigs generated by rapid assembly methods which do not include a consensus step. The goal of Racon is to generate genomic consensus which is of similar or better quality compared to the output generated by assembly methods which employ both error correction and consensus steps, while providing a speedup of several times compared to those methods. It supports data produced by both Pacific Biosciences and Oxford Nanopore Technologies.

Racon can be used as a polishing tool after the assembly with **either Illumina data or data produced by third generation of sequencing**. The type of data inputed is automatically detected.

Racon takes as input only three files: contigs in FASTA/FASTQ format, reads in FASTA/FASTQ format and overlaps/alignments between the reads and the contigs in MHAP/PAF/SAM format. Output is a set of polished contigs in FASTA format printed to stdout. All input files **can be compressed with gzip** (which will have impact on parsing time).

Racon can also be used as a read error-correction tool. In this scenario, the MHAP/PAF/SAM file needs to contain pairwise overlaps between reads **including dual overlaps**.

A **wrapper script** is also available to enable easier usage to the end-user for large datasets. It has the same interface as racon but adds two additional features from the outside. Sequences can be **subsampled** to decrease the total execution time (accuracy might be lower) while target sequences can be **split** into smaller chunks and run sequentially to decrease memory consumption. Both features can be run at the same time as well.

## Dependencies
1. gcc 4.8+ or clang 3.4+
2. cmake 3.2+

### CUDA Support
1. gcc 5.0+
2. cmake 3.10+
4. CUDA 9.0+

## Installation
To install Racon run the following commands:

```bash
git clone --recursive https://github.com/lbcb-sci/racon.git racon
cd racon
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

After successful installation, an executable named `racon` will appear in `build/bin`.

Optionally, you can run `sudo make install` to install racon executable to your machine.

***Note***: if you omitted `--recursive` from `git clone`, run `git submodule update --init --recursive` before proceeding with compilation.

To build unit tests add `-Dracon_build_tests=ON` while running `cmake`. After installation, an executable named `racon_test` will be created in `build/bin`.

To build the wrapper script add `-Dracon_build_wrapper=ON` while running `cmake`. After installation, an executable named `racon_wrapper` (python script) will be created in `build/bin`.

### Installation in Conda environment
Use these directions to build into a conda environment for the T2T Automated pipeline.

```bash
conda create -n name
conda activate name
conda install cmake git

git clone --recursive https://github.com/lbcb-sci/racon.git racon
cd racon
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### CUDA Support
Racon makes use of [NVIDIA's GenomeWorks SDK](https://github.com/clara-parabricks/GenomeWorks) for CUDA accelerated polishing and alignment.

To build `racon` with CUDA support, add `-Dracon_enable_cuda=ON` while running `cmake`. If CUDA support is unavailable, the `cmake` step will error out.
Note that the CUDA support flag does not produce a new binary target. Instead it augments the existing `racon` binary itself.

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Release -Dracon_enable_cuda=ON ..
make
```

## Usage
Usage of `racon` is as following:

    racon [options ...] <sequences> <overlaps> <target sequences>

        # default output is stdout
        <sequences>
            input file in FASTA/FASTQ format (can be compressed with gzip)
            containing sequences used for correction
        <overlaps>
            input file in MHAP/PAF/SAM format (can be compressed with gzip)
            containing overlaps between sequences and target sequences
        <target sequences>
            input file in FASTA/FASTQ format (can be compressed with gzip)
            containing sequences which will be corrected

    options:
        options:
        -u, --include-unpolished
            output unpolished target sequences
        -f, --fragment-correction
            perform fragment correction instead of contig polishing
            (overlaps file should contain dual/self overlaps!)
        -w, --window-length <int>
            default: 500
            size of window on which POA is performed
        -q, --quality-threshold <float>
            default: 10.0
            threshold for average base quality of windows used in POA
        -e, --error-threshold <float>
            default: 0.3
            maximum allowed error rate used for filtering overlaps
        --no-trimming
            disables consensus trimming at window ends
        
        *** Not available in main Racon
        -L, --liftover <string>
            default: ''
            optional prefix of the output liftover files which convert
            the draft sequence to the output consensus. PAF and VCF files
            are always written with this prefix, and SAM can optionally
            be written if the -S option is provided.            VCF, PAF, SAM. Format is determined from extension.
        -S, --liftover-sam
            Used only in combination with the -L option, this writes out
            a SAM formatted alignment of the polished sequences vs the draft.
        *** Not available in main Racon
        
        -m, --match <int>
            default: 3
            score for matching bases
        -x, --mismatch <int>
            default: -5
            score for mismatching bases
        -g, --gap <int>
            default: -4
            gap penalty (must be negative)
        -B, --bed <str>
            default: ''
            path to a BED file with regions to polish
        -t, --threads <int>
            default: 1
            number of threads
        --version
            prints the version number
        -h, --help
            prints the usage

    only available when built with CUDA:
        -c, --cudapoa-batches <int>
            default: 0
            number of batches for CUDA accelerated polishing per GPU
        -b, --cuda-banded-alignment
            use banding approximation for polishing on GPU. Only applicable when -c is used.
        --cudaaligner-batches <int>
            default: 0
            number of batches for CUDA accelerated alignment per GPU
        --cudaaligner-band-width <int>
            default: 0
            Band width for cuda alignment. Must be >= 0. Non-zero allows user defined
            band width, whereas 0 implies auto band width determination.

`racon_test` is run without any parameters.

Usage of `racon_wrapper` equals the one of `racon` with two additional parameters:

    ...
    options:
        --split <int>
            split target sequences into chunks of desired size in bytes
        --subsample <int> <int>
            subsample sequences to desired coverage (2nd argument) given the
            reference length (1st argument)
        ...
