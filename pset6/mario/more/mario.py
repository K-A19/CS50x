from cs50 import get_int


def main():

    height = get_positive_int()
    draw_pyramid(height)


def get_positive_int():

    height = -1

    # Repeats prompt for input until it is valid
    while height < 1 or height > 8:
        height = get_int("Height: ")

    return height


def draw_pyramid(height):  # Draws pyramid body of height inputted by the user
    for i in range(1, (height + 1)):
        print(" " * (height - i), end="")
        print("#" * i, end="")
        print("  ", end="")
        print("#" * i)


# Runs main last to make sure all functions have been declared
main()
