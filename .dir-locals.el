;; Select CMake and debugger settings for the local platform or Podman remote.
((nil
  . ((eval
      . (let* ((remote (file-remote-p default-directory))
               (preset
                (cond
                 (remote "ubuntu-x64-debug")
                 ((eq system-type 'darwin) "macos-arm64-debug")
                 ((eq system-type 'gnu/linux) "ubuntu-x64-debug")
                 (t (error "No CMake preset for %s" system-type))))
               (build-dir (concat "build-cmake/" preset))
               (debug-adapter (if (and (not remote)
                                       (eq system-type 'darwin))
                                  'lldb-dap
                                'gdb)))
          (setq-local compile-command
                      (format "cmake --preset %s && cmake --build --preset %s --parallel"
                              preset preset))
          (setq-local dape-command
                      (if (eq debug-adapter 'lldb-dap)
                          `(lldb-dap :program
                                     ,(concat build-dir "/unix-server"))
                        `(gdb defer-launch-attach t
                              :program ,(concat build-dir "/unix-server"))))
          (setq-local eglot-server-programs
                      `(((c++-mode c++-ts-mode)
                         . ("clangd"
                            ,(concat "--compile-commands-dir=" build-dir))))))))))
