# GeNETiCC ( C Library)
#### `V0.5.4`

__**A C library which implements generic functions for working with [arrays](https://github.com/AddioElectronics/GeNETiCC/wiki/API-Array), [lists](https://github.com/AddioElectronics/GeNETiCC/wiki/API-List), and more(coming eventually), in a style that implements syntactic sugar that slightly resembles C#.**__

`Please Note: This is more of a proof of concept, and is not intended to be a production ready library.`


## Notes

`The README and WIKI are currently in a state of disarray. They will be finished eventually.`

The library is in an early state but almost production ready. It is very hard to test every permutation of each Macro/Function, so it would be much appreciated if you could report any bugs so I am able to fix them ASAP. Also feel free to post any issues, suggestions or questions you may have, this library is not like most other C libraries, and I want to make the documentation as clear as possible to avoid any confusion you or anyone else may have. My goal is to have this very approachable for coders of any skill level, just like C#.

One thing about DMA, from the small tests I've done everything is working smoothly, but I have not had the chance to test it while other channels are in use. It is some of the least finished code in the repo, I have not had a chance to add comments and perform a clean up, so just be careful when using it for any critical projects.


## Contents

* [Reference API](https://github.com/AddioElectronics/GeNETiCC/wiki)
* [Change Log](https://github.com/AddioElectronics/GeNETiCC/blob/master/ChangeLog.md)
* [Usage](#usage)
  * [Macros](#macros)
  * [Functions](#functions_internal)
  * [Type Specific Functions](#type_specific_functions)
  * [DMA](#dma)
* [Contributing](#contributing)
* [License](#license)
* [Author](#author)

## Usage <a name="usage"/>

#### [Documentation and Reference API](https://github.com/AddioElectronics/GeNETiCC/wiki)

Just import the desired headers and source files into your project, and include the headers.

``` C
//Master header, will include all components.
#include "addio/using/using_Addio.GeNETiCC.h"

//If you wish to include specific components,
//it will import everything needed to use them.
#include "addio/using/using_Addio.GeNETiCC.collections.list.h"
```

Add the "addio/using" folder into your project's included directories to shorten the path.

``` C
#include "using_Addio.GeNETiCC.h"
```

### Macros (Main Usage) <a name="macros"/>

#### [This info has been moved to the WIKI](https://github.com/AddioElectronics/GeNETiCC/wiki)

Macros are the intended way to use this library, they call into internal functions after using pre-processor "tricks" to implement function overloading, and generic casting.


### Functions (Internal) <a name="functions_internal"/>

#### [This info has been moved to the WIKI](https://github.com/AddioElectronics/GeNETiCC/wiki)

Most functions are for internal use, to be called from the macros. They are still available for use, if you choose to use them, but you will miss out on the features this library has to offer.

### Type Specific Functions (Internal) <a name="type_specific_functions"/>

#### [This info has been moved to the WIKI](https://github.com/AddioElectronics/GeNETiCC/wiki)

Type specific functions can be disabled (in most cases) by setting the `GENETICC_OPTIMIZATION_LEVEL` to `GENETICC_OPTIMIZATION_LEVEL_SIZE.`

In cases where performance is an issue for a certain data type, functions for that specific data type can be added to squeeze as much performance as possible.

As of now, only a few type specific functions have been added, typically for floating point types. In the future most Macros will contain them, if you nee
 
After creating the function, the internal macro will need a minor modification to allow the "public" macro to call into the function.

### DMA <a name="dma"/>

#### [Info In the Wiki](https://github.com/AddioElectronics/GeNETiCC/wiki/DMA)

## Contributing

Would you like to contribute to this project? [CONTRIBUTING.md](https://github.com/AddioElectronics/GeNETiCC/blob/master/Contributing.md)

## License

GeNETiCC is released under the [MIT License.](http://www.opensource.org/licenses/MIT)

## Author

- Author   : Addio from Addio Electronics (Canada)
- Website  : www.Addio.io

