// bin: -Wall -Wextra -lxbps

/*
 ____  ____            _
|  _ \|  _ \ __ _  ___| | _____ _ __
| | | | |_) / _` |/ __| |/ / _ \ '__|
| |_| |  __/ (_| | (__|   <  __/ |
|____/|_|   \__,_|\___|_|\_\___|_|

This configuration has only been tested on Void Linux.
*/

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
        "base-devel base-system linux linux-firmware-broadcom",
        "grub grub-x86_64-efi",
        amd("linux-firmware-amdgpu lvulkan-radeon linux-firmware-radeon"), // not checked
        nvidia("linux-firmware-nvidia"),
        intel(
              "linux-firmware-intel",
              "mesa-vulkan-intel",
              "intel-gmmlib intel-ucode",
              "intel-media-driver",
              "void-repo-nonfree"
        ),
		"libxbps-devel",
        "bash zsh", // bash-completion
        "ledger",
        "isync mu",
        "github-cli",
        "hunspell-pt_BR hunspell-en",
        "curl ImageMagick openssh openssl sqlite ueberzug", // jq
        "dbus",
        "efibootmgr",
        "elogind",
        "git gnupg less",
        "libgccjit",
        "libotf",
        "libtool",
        "libvorbis",
        "m17n-lib",
        "make",
        "man-pages",
        "mpv",
        "neovim",
        "ntp",
        "openresolv",
        "openssh",
        "openssl",
        "pass pass-otp zbar",
        "pkgconf",
        "poppler-devel",
        "poppler-glib-devel",
        "restic rsync",
        "sqlite",
        "sxhkd",
        "tar gzip zip unzip",
        "ufw",
        "pandoc",
        "rofi",
        "wireguard-tools",
        "turnstile",
        "wget",
        "which",
        "wpa_supplicant",
        "xclip",
        "xfsprogs",
        "xdg-utils xdg-desktop-portal",
        "ffmpeg sox",
        "pinentry-tty",
        "zoxide btop fzf direnv",
        "mesa",
#ifdef DESKTOP_MODE
        "fuse",
        "i3",
        "i3status",        
        "cairo-devel",
        "emacs-gtk3",
		"dunst",
        "gimp",
        "ttf-ubuntu-font-family noto-fonts-emoji",
        "pipewire pulseaudio wireplumber",
        "alsa-utils",
        "adwaita-icon-theme adwaita-icon-theme",
        "yt-dlp python3-mutagen",
        "zathura zathura-pdf-mupdf",
        // "libreoffice-still hunspell",
        // "pcmanfm tumbler ffmpegthumbnailer",
        // "polkit udiskie udisks2",
        // "qutebrowser",
        // "shotcut sox",
        // "tenacity",
        // "thunderbird",
        // "wine",

        virtual_machine("bridge-utils dnsmasq dosfstools libvirt lxc qemu-full swtpm virt-manager virt-viewer"),

        wayland(
            "fuzzel pavucontrol swaybg cliphist xdg-user-dirs ydotool foot",
            // "grim satty slurp wf-recorder",
            "cpio gsettings-desktop-schemas libva-utils lm_sensors wl-clipboard wlr-randr",
        ),

        x11("xorg libXft-devel xorg-server xorg-server-common xorg-server-xnest xorg-server-xvfb xorg-server-devel",
            "xsel xclip xdotool",

            "xwallpaper zenity dconf dmenu redshift sxhkd", // picom conky
            "ksnip",
            // "obs-studio",
        )

        steam(
           "gamemode steam",
        ),        
#endif

#ifdef PROGRAMMER_MODE
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
       "ttf-jetbrains-mono-nerd",
       "anydesk",
       "opendoas",
	   "st",
       // "gf2-git",
       // "brave-origin",
       // odin("odin-git ols-git odinfmt"),
       // x11("zoomer"),       
       NULL,
};
// clang-format on

int main(int argc, char **argv) {
    DPacker_Interface interface;
    VOID_CONFIG.xbps_src_root = "/home/aoc/void-packages";
    VOID_CONFIG.user = "aoc";

    interface.init = dpacker_xbps_init;
    interface.collect = dpacker_xbps_collect;
    return dpacker(interface, native, void_packages, argc, argv);
}
