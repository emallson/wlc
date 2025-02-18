.. |build| image:: http://build.cloudef.pw/build/wlc/master/linux%20x86_64/current/status.svg
.. _build: http://build.cloudef.pw/build/wlc/master/linux%20x86_64

:IRC: #orbment @ freenode
:Build: |build|_

FEATURES
--------

+------------------+-----------------------+
| Backends         | DRM, X11              |
+------------------+-----------------------+
| Renderers        | EGL, GLESv2           |
+------------------+-----------------------+
| TTY session      | logind, legacy (suid) |
+------------------+-----------------------+
| Input            | libinput, xkb         |
+------------------+-----------------------+
| Monitor          | Multi-monitor, DPMS   |
+------------------+-----------------------+
| Hotplugging      | udev                  |
+------------------+-----------------------+
| Xwayland         | Supported             |
+------------------+-----------------------+
| Clipboard        | Partially working     |
+------------------+-----------------------+
| Drag'n'Drop      | Not implemented       |
+------------------+-----------------------+

EXAMPLE
-------

.. code:: c

    // For more functional example see example/example.c

    #include <stdlib.h>
    #include <wlc/wlc.h>

    static bool
    view_created(wlc_handle view)
    {
       wlc_view_bring_to_front(view);
       wlc_view_focus(view);
       return true;
    }

    static void
    view_focus(wlc_handle view, bool focus)
    {
       wlc_view_set_state(view, WLC_BIT_ACTIVATED, focus);
    }

    int
    main(int argc, char *argv[])
    {
       wlc_set_view_created_cb(view_created);
       wlc_set_view_focus_cb(view_focus);

       if (!wlc_init2())
          return EXIT_FAILURE;

       wlc_run();
       return EXIT_SUCCESS;
    }

ENV VARIABLES
-------------

``wlc`` reads the following env variables.

+----------------------+------------------------------------------------------+
| ``WLC_DRM_DEVICE``   | Device to use in DRM mode. (card0 default)           |
+----------------------+------------------------------------------------------+
| ``WLC_SHM``          | Set 1 to force EGL clients to use shared memory.     |
+----------------------+------------------------------------------------------+
| ``WLC_OUTPUTS``      | Number of fake outputs in X11 mode.                  |
+----------------------+------------------------------------------------------+
| ``WLC_XWAYLAND``     | Set 0 to disable Xwayland.                           |
+----------------------+------------------------------------------------------+
| ``WLC_LIBINPUT``     | Set 1 to force libinput. (Even on X11)               |
+----------------------+------------------------------------------------------+
| ``WLC_REPEAT_DELAY`` | Keyboard repeat delay.                               |
+----------------------+------------------------------------------------------+
| ``WLC_REPEAT_RATE``  | Keyboard repeat rate.                                |
+----------------------+------------------------------------------------------+
| ``WLC_DEBUG``        | Enable debug channels (comma separated)              |
+----------------------+------------------------------------------------------+

KEYBOARD LAYOUT
---------------

You can set your preferred keyboard layout using ``XKB_DEFAULT_LAYOUT``.

See xkb documentation for more details.

RUNNING ON TTY
--------------

If you have ``logind``, you don't have to do anything.

Without ``logind`` you need to suid your binary to root user.
The permissions will be dropped runtime.

ISSUES
------

Submit issues on this repo if you are developing with ``wlc``.

As a user of compositor, report issues to their corresponding issue trackers.

BUILDING
--------

You will need following makedepends:

- cmake
- git

And the following depends:

- pixman
- wayland 1.7+
- wayland-protocols 1.0+ [1]
- libxkbcommon
- udev
- libinput
- libx11 (X11-xcb, Xfixes)
- libxcb (xcb-ewmh, xcb-composite, xcb-xkb, xcb-image, xcb-xfixes)
- libgbm (usually provided by mesa in most distros)
- libdrm
- libEGL (GPU drivers and mesa provide this)
- libGLESv2 (GPU drivers and mesa provide this)

1: Also bundled as submodule. Using submodule is recommended, as updates to unstable protocols won't break build. To force building from submodule even though wayland-protocols is installed in system use CMake option -DSOURCE_WLPROTO=ON.

And optionally:

- dbus (for logind support)
- systemd (for logind support)

For weston-terminal and other wayland clients for testing, you might also want to build weston from git.

You can build bootstrapped version of ``wlc`` with the following steps.

.. code:: sh

    git submodule update --init --recursive # - initialize and fetch submodules
    mkdir target && cd target               # - create build target directory
    cmake -DCMAKE_BUILD_TYPE=Upstream ..    # - run CMake
    make                                    # - compile

    # You can now run (Ctrl-Esc to quit)
    ./example/example

PACKAGING
---------

For now you can look at the `AUR recipe <https://aur.archlinux.org/packages/wlc-git/>`_ for a example.

CONTRIBUTING
------------

See the `CONTRIBUTING <CONTRIBUTING.rst>`_ for more information.

BINDINGS
--------

- `ocaml-wlc <https://github.com/Armael/ocaml-wlc>`_ - OCaml (experimental)
- `go-wlc <https://github.com/mikkeloscar/go-wlc>`_ - Go
- `rust-wlc <https://github.com/Immington-Industries/rust-wlc>`_ - Rust

SOFTWARE USING WLC
------------------

- `orbment <https://github.com/Cloudef/orbment>`_ - Modular Wayland compositor
- `ocaml-loliwm <https://github.com/Armael/ocaml-loliwm>`_ - Translation of loliwm to OCaml
- `sway <https://github.com/SirCmpwn/sway>`_ - i3-compatible window manager for Wayland

SIMILAR SOFTWARE
----------------

- `swc <https://github.com/michaelforney/swc>`_ - A library for making a simple Wayland compositor
- `libwlb <https://github.com/jekstrand/libwlb>`_ - A Wayland back-end library
- `libweston <https://github.com/giucam/weston/tree/libweston>`_ - Weston as a library
