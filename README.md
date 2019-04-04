# ZED2RGBDImage

This code can save RGB images and Depth images from the ZED.

## Requirements
- OpenCV
- CUDA10.0

## How to use
```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./ZED2RGBDImage
```

### If you have a svo file ...
```
$ ./ZED2RGBDImage path_to_svo_file
```

## Output
```
build
└── images
    ├── depth
    |   ├── 0.png
    |   ├── 1.png
    |   ...
    ├── right
    |   ├── 0.png
    |   ├── 1.png
    |   ...
    └── right
        ├── 0.png
        ├── 1.png
        ...
```


#### Depth Image
<img src="https://user-images.githubusercontent.com/26996041/55551387-a2df8c80-5715-11e9-8411-f169cd6cbcff.png" width=40% title="left">

#### Left Image
<img src="https://user-images.githubusercontent.com/26996041/55551155-087f4900-5715-11e9-939c-10fe8003685f.png" width=40% title="left">

#### Right Image
<img src="https://user-images.githubusercontent.com/26996041/55551345-8b080880-5715-11e9-9382-379f28a02f37.png" width=40% title="left">