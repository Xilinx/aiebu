# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

"""
ANSI escape sequences for terminal colors and styles.
This module provides ANSI color and style constants as a replacement for colorama.
"""


class Fore:
    """Foreground colors using ANSI escape sequences."""
    BLACK = '\033[30m'
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    MAGENTA = '\033[35m'
    CYAN = '\033[36m'
    WHITE = '\033[37m'


class Style:
    """Text styles using ANSI escape sequences."""
    RESET_ALL = '\033[0m'
    BRIGHT = '\033[1m'
    DIM = '\033[2m'
    NORMAL = '\033[22m'


def test_ansi():
    """Test and display all available ANSI colors and styles."""
    print("\n=== ANSI Color and Style Test ===\n")
    
    # Test foreground colors
    print("Foreground Colors:")
    for color_name in dir(Fore):
        if not color_name.startswith('_'):
            color_code = getattr(Fore, color_name)
            print(f"  {color_code}{color_name}{Style.RESET_ALL}")
    
    print("\nText Styles:")
    # Test text styles
    styles = [
        ('RESET_ALL', Style.RESET_ALL),
        ('BRIGHT', Style.BRIGHT),
        ('DIM', Style.DIM),
        ('NORMAL', Style.NORMAL),
    ]
    
    for style_name, style_code in styles:
        if style_name == 'RESET_ALL':
            print(f"  {Style.BRIGHT}{style_name}{Style.RESET_ALL} (use to reset)")
        else:
            print(f"  {style_code}{style_name}{Style.RESET_ALL}")
    
    # Test combined colors and styles
    print("\nCombined Examples:")
    print(f"  {Fore.GREEN}{Style.BRIGHT}Green + Bright{Style.RESET_ALL}")
    print(f"  {Fore.BLUE}Blue + Normal{Style.RESET_ALL}")
    print(f"  {Fore.MAGENTA}{Style.DIM}Magenta + Dim{Style.RESET_ALL}")
    print(f"  {Fore.CYAN}Cyan Normal{Style.RESET_ALL}")
    print()


if __name__ == '__main__':
    test_ansi()
