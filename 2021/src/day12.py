import pandas as pd


def count_lowercase(seq):
    low_cnt = {
        u: sum([ n == u for n in seq])
        for u in set(seq) if u != "start" and u.islower()
    }
    return low_cnt


def find_valid_paths(conn_mat, seq=[ "start", ], allow_repeat=False):
    ret = []
    vertices = conn_mat[seq[-1]]
    vertices = vertices[vertices]
    for v in vertices.index:
        new_seq = [ n for n in seq ]
        new_seq.append(v)
        low_cnt = count_lowercase(new_seq)
        repeat = allow_repeat and (
                all([ c < 2 for c in low_cnt.values() ])
                or sum([ c == 2 for c in low_cnt.values() ]) == 1
        )
        if v == "end":
            ret.append(new_seq)
        elif v == "start":
            continue
        elif v.isupper() or v not in seq or repeat:
            sub_paths = find_valid_paths(conn_mat, new_seq, allow_repeat)
            ret.extend(sub_paths)

    return ret


def load_input(path):
    with open(path, 'r') as f:
        rows = f.readlines()

    rows = [ r.strip().split('-') for r in rows ]
    nodes = set([ r[0] for r in rows ] + [ r[1] for r in rows ])
    ret = pd.DataFrame(False, index=nodes, columns=nodes, dtype=bool)
    for n1, n2 in rows:
        ret.at[n1, n2] = True
        ret.at[n2, n1] = True

    return ret


def print_results(input, part):
    if part == 1:
        ret = len(find_valid_paths(input))
    elif part == 2:
        ret = len(find_valid_paths(input, allow_repeat=True))

    print("Answer for part {} is {}".format(part, ret))


if __name__ == "__main__":
    # Not using networkx because it only supports simple paths,
    # i.e. paths where a node cannot be repeated.
    # Using a connectivity matrix instead.
    test_nodes = [ "start", "A", "b", "c", "d", "end", ]
    test_input = pd.DataFrame([
        [ 0, 1, 1, 0, 0, 0, ],
        [ 1, 0, 1, 1, 0, 1, ],
        [ 1, 1, 0, 0, 1, 1, ],
        [ 0, 1, 0, 0, 0, 0, ],
        [ 0, 0, 1, 0, 0, 0, ],
        [ 0, 1, 1, 0, 0, 0, ],
    ], index=test_nodes, columns=test_nodes, dtype=bool)
    assert len(find_valid_paths(test_input)) == 10
    assert len(find_valid_paths(test_input, allow_repeat=True)) == 36

    input_path = "data/input-day12.txt"
    input_data = load_input(input_path)
    print_results(input_data, 1)
    print_results(input_data, 2)

