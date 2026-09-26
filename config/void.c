/*
 ____  ____            _
|  _ \|  _ \ __ _  ___| | _____ _ __
| | | | |_) / _` |/ __| |/ / _ \ '__|
| |_| |  __/ (_| | (__|   <  __/ |
|____/|_|   \__,_|\___|_|\_\___|_|

This configuration has only been tested on Void Linux.
*/

#define NONFREE
#define TELEMETRY

// === Processor ===
// #define AMD
// #define NVIDIA
#define INTEL
// #define VM

// === Display ===
#define X11
// #define WAYLAND

// === SOFTWARE ===
#define ANYDESK
#define EMACS
// #define STEAM
// #define VIRTUAL_MACHINE

// === LANGUAGES ===
#define C
// #define C3
// #define D
// #define ELIXIR
// #define ERLANG
// #define GO
// #define HASKELL
// #define JAVA
// #define JS
// #define KOTLIN
#define LUA
// #define OCAML
#define ODIN
#define PYTHON
// #define RUBY
// #define RUST
#define SHELL
// #define ZIG

// END OF DEFINES

#define PROGRAMMER_MODE
#define DESKTOP_MODE

#include "flags.h"
#include "dpacker.h"
#include "xbps.h"

// clang-format off
char *native[] = {
    nonfree("void-repo-nonfree"),

    // Base system
    "base-devel base-system",
    // Kernel
    "linux linux-firmware-broadcom",
    // Bootloader
    "grub grub-x86_64-efi efibootmgr",
    "mesa",
    // Filesystem
    "xfsprogs",

    // Processors
    amd("linux-firmware-amd mesa-vulkan-radeon"),
    nvidia(nonfree("linux-firmware-nvidia mesa-vulkan-nouveau")),
    intel(
        "linux-firmware-intel mesa-vulkan-intel intel-gmmlib intel-media-driver",
        nonfree("intel-ucode")
    ),

    // Shell
    "bash zsh", // bash-completion
    "less tree grep tar zip unzip gzip which curl wget",
    "zoxide fzf direnv", // btop

    // Accounting
    "ledger",

    // Mail
    "isync mu4e",

    // Encryption, signing
    "gnupg pinentry-tty",
    "openssl",

    // Services
    "ntp",
    "ufw",
    "wireguard-tools openresolv",
    "openssh",

    // Multimedia
    "mpv ImageMagick poppler-utils poppler",
    "ffmpeg sox",

    // Passwords
    "pass pass-otp zbar",

    // Backup
    "restic rsync",

    // Wi-Fi
    "wpa_supplicant",

    // Document converter
    "pandoc",

    // File Manager
    // "ranger ueberzug",

    // Downloader
    "yt-dlp python3-mutagen",

#ifdef DESKTOP_MODE
    // Services
    "turnstile",
    "dbus",
    "elogind",
	"dunst",
    "pipewire pulseaudio wireplumber alsa-utils",

    // XDG
    "xdg-utils xdg-desktop-portal",

    // Menu
    "rofi",

    // Bindings
    // "sxhkd",

    // AppImages
    "fuse",

    // Text Editor
    "emacs-gtk3",
    "hunspell-pt_BR hunspell-en", // Spellchecker for Emacs

    // Multimedia
    "gimp",
    // "shotcut",
    // "tenacity",

    // Fonts
    "ttf-ubuntu-font-family noto-fonts-emoji",

    // Icon & Theme
    "adwaita-icon-theme adwaita-icon-theme",

    // PDF
    "zathura zathura-pdf-mupdf",

    // Windows
    // "wine",

    virtual_machine("bridge-utils dnsmasq dosfstools libvirt lxc qemu-full swtpm virt-manager virt-viewer"),

    wayland(
        "fuzzel pavucontrol swaybg xdg-user-dirs ydotool foot",
        // "grim satty slurp wf-recorder",
        "cpio gsettings-desktop-schemas libva-utils lm_sensors wl-clipboard wlr-randr",
        // wl-copy wl-paste cliphist
    ),

    x11(
        "xorg libXft-devel xorg-server xorg-server-common xorg-server-xnest xorg-server-xvfb xorg-server-devel",
        "xsel xclip xdotool",

        "xwallpaper zenity dconf dmenu redshift sxhkd", // picom conky
        "flameshot",
        "xclip",
        "i3 i3status",
        // "obs-studio",
    )

    nonfree(steam(
       "gamemode steam",
    )),
#endif

#ifdef PROGRAMMER_MODE
    // Development
	"libxbps-devel", // for dpacker.h
    telemetry("github-cli"),
    "git",
    "pkgconf",
    "libgccjit",
    "libotf",
    "libtool",
    "libvorbis",
    "m17n-lib",
    "make",
    "man-pages",
    "sqlite",
    "cairo-devel",

    // Languages
    c        ("clang clang-tools-extra gcc gdb libtool make mold valgrind tcc"), // meson cmake ninja lldb
    c3       ("c3c"),
    d        ("dmd dfmt"),
    elixir   ("elixir"),
    erlang   ("erlang"),
    go       ("go"),
    haskell  ("ghc"),
    java     ("openjdk"),
    js       ("nodejs npm"),
    kotlin   ("kotlin"),
    lua      ("StyLua"),
    ocaml    ("ocaml"),
    python   ("python imath pystring python3-BeautifulSoup4 python3-six"),
    ruby     ("ruby"),
    rust     ("rust rust-analyzer"),
    shell    ("shfmt"), // shellcheck
    zig      ("zig zls"),
#endif // PROGRAMMER_MODE
    NULL,
};
// clang-format on

// clang-format off
char *void_packages[] = {
    // == my ports
    "ttf-jetbrains-mono-nerd",
    "gf2",
    "tinypass",

	// "odin",
    // "brave-origin",
    // odin("odin-git ols-git odinfmt"),
    // x11("zoomer"),

    // == void-packages
    "anydesk",
    "opendoas",
    "st",
    NULL,
};
// clang-format on

int main(int argc, char **argv) {
    DPacker_Interface interface;
    VOID_CONFIG.xbps_src_root = "/home/aoc/void-packages";

    interface.init = dpacker_xbps_init;
    interface.collect = dpacker_xbps_collect;
    return dpacker(interface, native, void_packages, argc, argv);
}
