{ pkgs ? import <nixpkgs> {} }: pkgs.mkShell {
  packages = [ 
    pkgs.kdePackages.qtbase
    pkgs.kdePackages.qtserialport
    pkgs.kdePackages.qtwebsockets
    
    pkgs.cmake
    pkgs.ninja
    pkgs.clang
  ];

  shellHook = ''
    export PS1="\[\e[31m\]hprc gs: \[\e[m\]\[\e[37m\]\W\[\e[m\]\[\e[37m\] \[\e[m\]\[\e[37m\]% \[\e[m\]"
  '';
}
