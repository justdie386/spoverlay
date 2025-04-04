Make sure to run the app with admin if you install it somewhere it doesn't have read/write access by default. Why? Because to make it so you only have to log in once, we save some data so we can re-use it another time.

On windows, to setup, run

```xmake```

```xmake install -o export```

open the export folder, and voila, you should have everything you need

running the app from

```xmake run```

will NOT be able to find the buttons, currently only have gotten the app tested on windows. Will test it soon on linux and FreeBSD.
