;; Ce fichier « manifeste » peut être passé à « guix package -m » pour reproduire
;; le contenu de votre profil. Son contenu est « symbolique » : il ne spécifie que les
;; noms des paquets. Pour reproduire exactement le même profil, vous devez aussi
;; retenir les canaux utilisés, ceux renvoyés par « guix describe ».
;; Voir la section « Répliquer Guix » dans le manuel.

(specifications->manifest
  (list "coreutils"
        "bash"
        "icecat"
        "gcc-toolchain"
        "make"
        "python-pandas"
        "python-scipy"
        "python"
        "jupyter"
        "emacs"))
