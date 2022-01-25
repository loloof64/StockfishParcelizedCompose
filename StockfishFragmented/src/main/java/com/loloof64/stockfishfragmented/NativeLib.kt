package com.loloof64.stockfishfragmented

class NativeLib {

    /**
     * A native method that is implemented by the 'stockfishfragmented' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'stockfishfragmented' library on application startup.
        init {
            System.loadLibrary("stockfishfragmented")
        }
    }
}