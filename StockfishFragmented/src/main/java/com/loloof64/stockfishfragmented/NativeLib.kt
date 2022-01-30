package com.loloof64.stockfishfragmented

class NativeLib {

    external fun newGame()

    external fun setPositionFromStart(moves: Array<String>)

    companion object {

        // Used to load the 'stockfishfragmented' library on application startup.
        init {
            System.loadLibrary("stockfishfragmented")
        }
    }
}