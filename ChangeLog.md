
# Change Log
All notable changes to this project will be documented in this file.

## [0.5.3] - 11/21/2021
  
Minor cleanup "..._generic" functions.

### Changed
  
* Replaced macro with __always_inline__. Macro only inlined for certain internal optimization levels.
* Removed intermediate generic_union_t variable, instead grabbing pointer from argument.

## [0.5.2] - 11/19/2021
  
Minor cleanup of DMA files.

### Added

* Template files used for adding new systems to DMA.
 
### Changed
  
* Moved system specific macros from geneticc_dma.h to its own file. (geneticc_dma_atmelstart_arm.h)

## [0.5.1] - 11/12/2021
  
Restructured folders and added "using" files.
 
### Added

* Using files. Headers which make including components easier.
 
### Changed
  
* Moved components that deal with collections of values to the "Collections" sub-directory. (Array, List...) 
 
## [0.5.0] - 11/1/2021
  
A lot has changed, and even more has been added since v0.1.0.
Upgrading the v0.5.0 will require changing the name of any calls to macros or functions in to the library.
 
### Added

* List Type (list_t)
* List Functions
* Predicates
* Actions
* DMA
* Way too much to include
 
### Changed
  
* Literally Everything 
 
## [0.1.0] - 10/1/2021
 
### Added
   
* Initial commit.
* Basic Array Functions(Contains, IndexOf, LastIndexOf)
