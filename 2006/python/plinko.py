import re

plinko_re = "><"

def run(grid):
    gridlines = [line.strip() for line in grid.split("\n") if line]
    width = len(gridlines[0])
    ordering = range(width)
    plinks = [0] * width
    for line in gridlines:
        for m in re.finditer(plinko_re, line):
            i = m.start()
            tmp = ordering[i]
            plinks[tmp] += 1
            ordering[i] = ordering[i+1]
            ordering[i+1] = tmp
    return dict(
        # ball -> (index, plinks)
        [(i, (ordering.index(i), plinks[i])) for i in range(width)]
    )

if __name__ == "__main__":
    GAME = """
    |><|
    ><||
    ><||
    ><||
    |><|
    ||><
    |><|
    |><|
    |><|
    """
