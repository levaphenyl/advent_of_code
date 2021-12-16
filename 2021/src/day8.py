SEGMAP = {
    0: "abcefg",
    1: "cf",
    2: "acdeg",
    3: "acdfg",
    4: "bcdf",
    5: "abdfg",
    6: "abdefg",
    7: "acf",
    8: "abcdefg",
    9: "abcdfg",
}


def count_segments():
    count = { k: len(v) for k, v in SEGMAP.items() }
    reverse_cnt = {}
    for k, v in count.items():
        if v in reverse_cnt:
            reverse_cnt[v].append(k)
        else:
            reverse_cnt[v] = [ k, ]

    return reverse_cnt


def digit_diff(segments1, segments2):
    diffchars = [ c for c in segments1 if c not in segments2 ]
    return "".join(diffchars)


def print_digit_diff():
    for k1, v1 in SEGMAP.items():
        for k2, v2 in SEGMAP.items():
            t = digit_diff(v1, v2)
            if len(t) < 2 and len(t) > 0:
                print("{}-{}: {}".format(k1, k2, t))


def replace_unique_values(values):
    segcount = count_segments()
    replaced = []
    for val in values:
        rep = [ segcount[len(d)][0] if len(segcount[len(d)]) == 1 else d
                for d in val ]
        replaced.append(rep)

    return replaced


def reverse_search(map, val):
    for k, v in map.items():
        if v == val:
            return k

    return None


def decode(patterns, values):
    ret = []
    segcount = count_segments()
    for i in range(len(values)):
        pattern = patterns[i]
        value = values[i]
        decode_map = {}
        while len(decode_map) < 10:
            for p in pattern:
                p = ''.join(sorted(p))
                if p not in decode_map:
                    possible = segcount[len(p)]
                    # Digits with unique number of segments: 1, 4, 7, 8.
                    if len(possible) == 1:
                        decode_map[p] = possible[0]

                    one = reverse_search(decode_map, 1)
                    four = reverse_search(decode_map, 4)
                    if len(p) == 6 and one is not None and four is not None:
                        # 9 contains the segments of 4.
                        if all([ s in p for s in four ]):
                            decode_map[p] = 9
                        # 0 contains the segments of 1 but not of 4.
                        elif all([ s in p for s in one ]):
                            decode_map[p] = 0
                        # 6 is the only left with 6 segments.
                        else:
                            decode_map[p] = 6

                    seven = reverse_search(decode_map, 7)
                    if 3 in possible and seven is not None:
                        # 3 contains the segments of 7.
                        if all([ s in p for s in seven ]):
                            decode_map[p] = 3
                            continue

                    three = reverse_search(decode_map, 3)
                    nine = reverse_search(decode_map, 9)
                    if len(p) == 5 and three is not None and nine is not None:
                        # Segment difference 9 - 3 --> b,
                        bee = digit_diff(nine, three)
                        # 5 is the only one with 5 segments containing b.
                        if bee in p:
                            decode_map[p] = 5
                        # 2 is the remaining one.
                        else:
                            decode_map[p] = 2

        decoded = [ str(decode_map[''.join(sorted(v))]) for v in value ]
        ret.append(int(''.join(decoded)))

    return ret


def count_digits(values):
    is_digit = [ isinstance(d, int) for val in values for d in val ]
    return sum(is_digit)


def load_data(path):
    with open(path, "r") as f:
        rows = f.readlines()

    input = [ r.strip().split(' | ') for r in rows ]
    return [ r[0].split() for r in input ], [ r[1].split() for r in input ]


def print_results(values, part, patterns=[]):
    if part == 1:
        replaced = replace_unique_values(values)
        ret = count_digits(replaced)
    elif part == 2:
        decoded = decode(patterns, values)
        ret = sum(decoded)

    print("Answer for part {}: {}.".format(part, ret))


if __name__ == "__main__":
    test_patterns = [
        [ "be", "cfbegad", "cbdgef", "fgaecd", "cgeb", "fdcge", "agebfd", "fecdb", "fabcd", "edb", ],
        [ "edbfga", "begcd", "cbg", "gc", "gcadebf", "fbgde", "acbgfd", "abcde", "gfcbed", "gfec", ],
        [ "fgaebd", "cg", "bdaec", "gdafb", "agbcfd", "gdcbef", "bgcad", "gfac", "gcb", "cdgabef", ],
        [ "fbegcd", "cbd", "adcefb", "dageb", "afcb", "bc", "aefdc", "ecdab", "fgdeca", "fcdbega", ],
        [ "aecbfdg", "fbg", "gf", "bafeg", "dbefa", "fcge", "gcbea", "fcaegb", "dgceab", "fcbdga", ],
        [ "fgeab", "ca", "afcebg", "bdacfeg", "cfaedg", "gcfdb", "baec", "bfadeg", "bafgc", "acf", ],
        [ "dbcfg", "fgd", "bdegcaf", "fgec", "aegbdf", "ecdfab", "fbedc", "dacgb", "gdcebf", "gf", ],
        [ "bdfegc", "cbegaf", "gecbf", "dfcage", "bdacg", "ed", "bedf", "ced", "adcbefg", "gebcd", ],
        [ "egadfb", "cdbfeg", "cegd", "fecab", "cgb", "gbdefca", "cg", "fgcdab", "egfdb", "bfceg", ],
        [ "gcafb", "gcf", "dcaebfg", "ecagb", "gf", "abcdeg", "gaef", "cafbge", "fdbac", "fegbdc", ],
    ]
    test_values = [
        [ "fdgacbe", "cefdb", "cefbgd", "gcbe" ],
        [ "fcgedb", "cgb", "dgebacf", "gc" ],
        [ "cg", "cg", "fdcagb", "cbg" ],
        [ "efabcd", "cedba", "gadfec", "cb" ],
        [ "gecf", "egdcabf", "bgf", "bfgea" ],
        [ "gebdcfa", "ecba", "ca", "fadegcb" ],
        [ "cefg", "dcbef", "fcge", "gbcadfe" ],
        [ "ed", "bcgafe", "cdgba", "cbgef" ],
        [ "gbdfcae", "bgc", "cg", "cgb" ],
        [ "fgae", "cfgab", "fg", "bagce" ],
    ]
    test_res2 = [ 8394, 9781, 1197, 9361, 4873, 8418, 4548, 1625, 8717, 4315, ]
    assert count_digits(replace_unique_values(test_values)) == 26
    assert decode(test_patterns, test_values) == test_res2

    input_path = "data/input-day8.txt"
    input_patn, input_vals = load_data(input_path)
    print_results(input_vals, 1)
    print_results(input_vals, 2, input_patn)

