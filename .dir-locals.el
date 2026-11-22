;; Intended for Emacs visiting /podman:<container>:/src/ (see README.md).
((nil
  . ((compile-command . "cmake --build --preset ubuntu-x64-debug --parallel")
     (dape-command
      . (gdb defer-launch-attach t
             :program "build-cmake/ubuntu-x64-debug/unix-server"))
     (eglot-server-programs
      . (((c++-mode c++-ts-mode)
          . ("clangd"
             "--compile-commands-dir=build-cmake/ubuntu-x64-debug")))))))
