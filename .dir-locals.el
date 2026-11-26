;; Select a CMake preset matching the local platform or Podman remote.
((nil
  . ((eval
      . (let* ((preset
                (cond
                 ((file-remote-p default-directory) "ubuntu-x64-debug")
                 ((eq system-type 'darwin) "macos-arm64-debug")
                 ((eq system-type 'gnu/linux) "ubuntu-x64-debug")
                 (t (error "No CMake preset for %s" system-type))))
               (build-dir (concat "build-cmake/" preset)))
          (setq-local compile-command
                      (format "cmake --build --preset %s --parallel" preset))
          (setq-local dape-command
                      `(gdb defer-launch-attach t
                            :program ,(concat build-dir "/unix-server")))
          (setq-local eglot-server-programs
                      `(((c++-mode c++-ts-mode)
                         . ("clangd"
                            ,(concat "--compile-commands-dir=" build-dir))))))))))
