# Crazy Eights

Basically uno, but with a normal deck of cards. A card can be played if the
rank is the same, suit is the same, or the card being played is an 8.

## Controls

|key|action|
----
|right arrow|move cursor to the right|
|left arrow|move cursor to the left|
|d|draw a card|
|space/enter/up/down arrow|play the card the cursor is over (up works for the lower player, down words for the upper player)|

The cursor looks like `==`

## Tests

There are 2 sets of tests `make test` and `make test-all`. `make test` runs the
tests for everything but the tcp stream. `make test-all` includes the tcp tests
which may fail if the echo server is down or unreachable or the port 8678 is
being used. The tcp tests may fail if run back to back because Linux doesn't
allow another process to bind to a port until a short delay passes. I do not
know if this also happens of MacOS or Windows.
