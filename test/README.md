# Unit Tests for KernelImageProcessor

This folder contains the unit tests for the project. These tests ensure that all core functionalities work as expected.

## Tests

### PNMFileHandlerFixture

This class tests the method from the class `PNMFileHandler`, used for loading and saving the image files.\
It checks if they works with all types of image files, with different bit depth and number of channels and it also tests the getters and setters in the `PixelImage` class.\
To do this it needs a temporary folder to save the files, so it creates it during the suite SetUp and destroys all the files in `TearDownTestSuite` method.

### ImageEditorFixture

Ensure all the image processing and editing methods of `ImageEditor` class work as intended.
During `SetUpTestSuite` it loads 4 example images and it creates an example Kernel.

### KernelTest

Simply checks if a `Kernel` object is constructed as expected.

