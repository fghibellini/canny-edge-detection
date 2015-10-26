
# Canny Edge Detection

## Description

Canny edge detection [C](https://en.wikipedia.org/wiki/C_%28programming_language%29) implementation based on [fast-edge](https://code.google.com/p/fast-edge/).
This is my semestral work for the university course _Effective Implemenation of Algorithms_.

It uses the [pgm](http://netpbm.sourceforge.net/doc/pgm.html) image file format.
The project includes a script to convert regular images to _pgm_ which uses [ImageMagic](http://www.imagemagick.org/script/index.php).

## Compilation & Usage

    # building
    make

    # convert image to PGM
    ./scripts/convert-to-pgm.sh images/racoon.png inputs/racoon.pgm

    # run program
    ./bin/canny inputs/racoon.pgm outputs/racoon.pgm

## Resources

- [Bill Green's excelent tutorial](http://dasl.mem.drexel.edu/alumni/bGreen/www.pages.drexel.edu/_weg22/can_tut.html)
- [fast-edge](https://code.google.com/p/fast-edge/) - C project this one is based on.
- [wikipedia](https://en.wikipedia.org/wiki/Canny_edge_detector)
- [some usefull lecture notes](http://www.cse.iitd.ernet.in/~pkalra/csl783/canny.pdf)

## License

[MIT](./LICENSE)
