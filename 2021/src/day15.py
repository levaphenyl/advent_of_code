import matplotlib.pyplot as plt
import networkx as nx
import pandas as pd


def grid2graph(grid):
    graph = nx.DiGraph()
    nodes = [ (i, c) for i in grid.index for c in grid.columns ]
    graph.add_nodes_from(nodes)
    for i in grid.index:
        for c in grid.columns:
            if i > grid.index[0]:
                graph.add_edge((i, c), (i - 1, c), weight=grid.at[i-1,c])
            if i < grid.index[-1]:
                graph.add_edge((i, c), (i + 1, c), weight=grid.at[i+1,c])
            if c > grid.columns[0]:
                graph.add_edge((i, c), (i, c - 1), weight=grid.at[i,c-1])
            if c < grid.columns[-1]:
                graph.add_edge((i, c), (i, c + 1), weight=grid.at[i,c+1])

    return graph


def lowest_risk(grid):
    graph = grid2graph(grid)
    start = (0, 0)
    end = (grid.index[-1], grid.columns[-1])
    path = nx.shortest_path(graph, start, end, "weight", 'dijkstra')
    risk = sum([ grid.at[i, c] for i, c in path[1:] ])
    return risk, path


def load_input(path):
    with open(path, 'r') as f:
        rows = f.readlines()

    rows = [ r.strip() for r in rows if len(r) > 1 ]
    rows = pd.DataFrame([ [ int(c) for c in r ] for r in rows ])
    return rows


def enlarge_input(grid, ratio=5):
    total_line = []
    for i in range(ratio):
        dupl = grid + i
        dupl[dupl > 9] -= 9
        total_line.append(dupl)

    total_line = pd.concat(total_line, axis=1)
    total_line.columns = range(ratio * grid.shape[1])
    total_grid = []
    for i in range(ratio):
        dupl = total_line + i
        dupl[dupl > 9] -= 9
        total_grid.append(dupl)

    total_grid = pd.concat(total_grid, axis=0, ignore_index=True)
    return total_grid


def print_results(grid, part):
    if part == 1:
        ret, _ = lowest_risk(grid)
    elif part == 2:
        grid = enlarge_input(grid)
        ret, _ = lowest_risk(grid)

    print("Answer for part {} is {}".format(part, ret))


if __name__ == "__main__":
    # Choosing the minimum of the neighbours as next point does not work.
    # Using a graph approach and computing all simple paths means combinatorial
    # explosion (10^271 combinations for a 100x100 grid).
    # Solution: use Dijkstra's algorithm: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
    test_input = pd.DataFrame([
        [ 1, 1, 6, 3, 7, 5, 1, 7, 4, 2, ],
        [ 1, 3, 8, 1, 3, 7, 3, 6, 7, 2, ],
        [ 2, 1, 3, 6, 5, 1, 1, 3, 2, 8, ],
        [ 3, 6, 9, 4, 9, 3, 1, 5, 6, 9, ],
        [ 7, 4, 6, 3, 4, 1, 7, 1, 1, 1, ],
        [ 1, 3, 1, 9, 1, 2, 8, 1, 3, 7, ],
        [ 1, 3, 5, 9, 9, 1, 2, 4, 2, 1, ],
        [ 3, 1, 2, 5, 4, 2, 1, 6, 3, 9, ],
        [ 1, 2, 9, 3, 1, 3, 8, 5, 2, 1, ],
        [ 2, 3, 1, 1, 9, 4, 4, 5, 8, 1, ],
    ], dtype="int32")
    test_graph = grid2graph(test_input)
    nx.draw(test_graph, with_labels=True, font_weight='bold')
    plt.show()
    assert lowest_risk(test_input)[0] == 40
    assert enlarge_input(test_input).max().max() == 9
    assert lowest_risk(enlarge_input(test_input))[0] == 315

    input_path = "data/input-day15.txt"
    input_grid = load_input(input_path)
    print_results(input_grid, 1)
    print_results(input_grid, 2)

