// bin: -Wall -Wextra -lalpm -DARCH -DMULTILIB -DARTIX

/*
 ____  ____            _
|  _ \|  _ \ __ _  ___| | _____ _ __
| | | | |_) / _` |/ __| |/ / _ \ '__|
| |_| |  __/ (_| | (__|   <  __/ |
|____/|_|   \__,_|\___|_|\_\___|_|

This configuration has only been tested on Artix Linux.
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
#include "alpm.h"

// clang-format off
char *pacman[] = {
        // Artix specific packages
        "artix-archlinux-support opendoas",
        "archlinux-keyring zram-generator pacman-contrib",
        "dinit elogind-dinit networkmanager-dinit pipewire-dinit pipewire-pulse-dinit",
        "wpa_supplicant-dinit turnstile-dinit wireplumber-dinit ntp-dinit openssh-dinit",
        "turnstile ntp",

        "base base-devel cryptsetup efibootmgr grub linux lld llvm lvm2 mesa xfsprogs",

        "linux-firmware-atheros linux-firmware-broadcom linux-firmware-cirrus",
        "linux-firmware-mediatek linux-firmware-other linux-firmware-realtek linux-firmware-whence",

        amd("linux-firmware-amdgpu lvulkan-radeon linux-firmware-radeon"),
        nvidia("linux-firmware-nvidia"),
        intel(
              "linux-firmware-intel",
              "intel-compute-runtime intel-gmmlib intel-ucode intel-graphics-compiler",
              "intel-media-driver vulkan-intel",
        )

        "bash", // bash-completion
        "btop zoxide", // ripgrep
        "curl imagemagick openssh openssl sqlite ueberzugpp", // jq
        "ffmpeg sox",
        "fastfetch",
        "fzf direnv",
        "git gnupg", // gitleaks git-filter-repo
        "libpulse pipewire pipewire-alsa pipewire-audio pipewire-jack pipewire-pulse gst-plugin-pipewire wireplumber",
        "mandoc man-pages",
        "ledger",
        "mpv",
        "neovim",
        "opusfile libmad libvorbis wavpack",
        "pass pass-otp zbar oath-toolkit",
        "ranger",
        "restic",
        "rsync",
        "tar gzip zip unzip",
        "tree less lsof wget which", // moreutils
        "ufw",
        "wpa_supplicant networkmanager",
        "zsh",
        "fuse2",
        "isync",
        "github-cli",
        // "android-tools gvfs gvfs-mtp mtpfs",
        // "bat eza fd sd",
        // "bluez bluez-utils",
        // "cmus playerctl",
        // "distrobox",
        // "docker docker-compose",
        // "harper",
        // "neomutt",
        // "newsboat",
        // "podman podman-compose podman-docker",
        // "prettier",
        // "rclone",
        // "yazi",

        // If EMACS is not defined, assume source compiling
        "aspell hunspell-en_us aspell-pt aspell-en hunspell libxaw libotf m17n-lib libgccjit", // global
        emacs("emacs"),

#ifdef DESKTOP_MODE
        "adwaita-cursors adwaita-icon-theme",
        "dunst libnotify",
        "gimp",
        "gnu-free-fonts noto-fonts-emoji ttf-jetbrains-mono-nerd",
        "pandoc-bin",
        "rofi",
        "wireguard-tools openresolv",
        "xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils",
        "yt-dlp python-mutagen",
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

        x11("xorg-server xorg-server-common xorg-server-xephyr xorg-server-xnest xorg-server-xvfb xorg-server-devel",
            "libxft xorg-xset xorg-xinit xorg-xrandr xsel xclip xdotool",

            "conky xwallpaper zenity dconf dmenu redshift sxhkd", // picom
            "ksnip",
            // "obs-studio",
        )

        steam(
           "gamemode steam",
        ),
        anydesk("minizip lsb-release"),

#endif // DESKTOP_MODE

#ifdef PROGRAMMER_MODE
        c        ("clang gcc gdb libtool make mold valgrind tcc"), // meson cmake ninja lldb
        c3       ("c3c"),
        d        ("dmd dfmt"),
        elixir   ("elixir"),
        erlang   ("erlang"),
        go       ("go"),
        haskell  ("ghc"),
        java     ("openjdk"),
        js       ("nodejs npm"),
        kotlin   ("kotlin"),
        lua      ("stylua"),
        ocaml    ("ocaml"),
        odin     ("odinfmt"),
        python   ("python imath pystring python-beautifulsoup4 python-lxml python-six"),
        ruby     ("ruby"),
        rust     ("rust rust-analyzer"),
        shell    ("shfmt"), // shellcheck
        zig      ("zig zls"),
#endif // PROGRAMMER_MODE

        NULL,
};
// clang-format on

// clang-format off
char *aur[] = {
    "gf2-git",
    "brave-origin-bin",
    // "jmtpfs",
    // "mutt-wizard",
    // "steghide",
    // "tomb",
    anydesk("anydesk-bin yp-tools"),
    x11("dwm st dwm-statusbar"),
    odin("odin-git ols-git"),
    "mu",
    x11("zoomer"),
    NULL,
};
// clang-format on

int main(int argc, char **argv) {
    DPacker_Interface interface;
    interface.init = dpacker_alpm_init;
    interface.collect = dpacker_alpm_collect;
    return dpacker(interface, pacman, aur, argc, argv);
}
