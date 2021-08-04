# Binary Space Partition

Binary Space Partitioning is a mainstay of the procedural generator's toolset.

Algorithmically, it's terribly simple:
1. Start with a space to partition (in this case a 2D rectangle of discrete dimensions)
2. Pick a dimension to partition (i.e. cut horizontally or vertically)
3. Pick a point at which to partition; this divides the space into two subregions.
4. Repeat recursively on these subregions until a minimum size is reached, or until some sort of probability function says to stop partitioning.

The real bread-and-butter of the algorithm is how the computer decides when to partition a region, and when not. The choice is specific to the use case; this implementation always partitions regions above a certain size, and partitions regions below that size with a 50% chance.

## Installation

Download [BSP.c](BSP.c) and compile it with any C compiler.
No nonstandard dependencies.

## Usage

Run the resulting executable with the following command-line parameters:
* width: the width of the starting region
* height: the height of the starting region
* seed (optional): the pRNG seed; if none is supplied, the current time will be used instead.

The partitioned starting region will be printed to ```stdout```.
