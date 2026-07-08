# Overview
Replay a chess game saved in PGN format. Code is implemented in C language. This code is mainly a practice for learning the C language.

Chess moves in PGN format don't specify the starting position of a move. Any implementation must therefore consider the current state of board and determine the correct starting position. This is rather hard to do since this calls for a complete implementation of chess game logic. The approach we take here is to convert the PGN format into the UCI coordinate system. Moves in the latter system contains the starting position as well. So we have to only take care of capture, castling and promotion.

One place to download PGN files of famous games is [PGN Mentor](https://www.pgnmentor.com/files.html).

To convert from PGN to UCI coordinate system we use the Windows executable from [pgn-extract](https://www.cs.kent.ac.uk/people/staff/djb/pgn-extract/) by David J. Barnes. Those interested can study its [source code](https://github.com/kentdjb/pgn-extract/). Conversion can be performed with the following command (assuming the PGN file of Kasparov):
`pgn-extract -Wuci Kasparov.pgn -o Kasparov.txt`
