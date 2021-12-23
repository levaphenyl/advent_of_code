def polymerize_step(chain, rules):
    new_chain = ''
    for i in range(len(chain) - 1):
        pair = chain[i:i+2]
        new_chain += pair[0]
        new_chain += rules[pair]

    new_chain += chain[-1]
    return new_chain


def count_elems(chain):
    count = {}
    for e in chain:
        if e in count:
            count[e] += 1
        else:
            count[e] = 1

    return count


def polymerize(init, rules, n_steps):
    # Exponential growth!
    # Do not use for n_steps > 20.
    chain = init
    for _ in range(n_steps):
        chain = polymerize_step(chain, rules)

    return chain


def polymerize_step_opt(pair_cnt, rules):
    new_state = { k: v for k, v in pair_cnt.items() }
    for pair, cnt in pair_cnt.items():
        if cnt:
            new_state[pair] -= cnt
            new_state[pair[0] + rules[pair]] += cnt
            new_state[rules[pair] + pair[1]] += cnt

    return new_state


def count_elems_opt(init, rules, n_steps):
    pair_cnt = { k: 0 for k in rules }
    for i in range(len(init) - 1):
        pair = init[i:i+2]
        pair_cnt[pair] += 1

    for _ in range(n_steps):
        pair_cnt = polymerize_step_opt(pair_cnt, rules)

    count = {}
    for pair, cnt in pair_cnt.items():
        if cnt:
            e = pair[0]
            if e in count:
                count[e] += cnt
            else:
                count[e] = cnt

    count[init[-1]] += 1
    return count



def load_input(path):
    with open(path, 'r') as f:
        rows = f.readlines()

    rows = [ r.strip() for r in rows if len(r) > 1 ]
    init = rows[0]
    rules = dict([ r.split(' -> ') for r in rows[1:] ])
    return init, rules


def print_results(init, rules, part):
    if part == 1:
        chain = polymerize(init, rules, 10)
        count = count_elems(chain)
    elif part == 2:
        count = count_elems_opt(init, rules, 40)

    emax = max(count.values())
    emin = min(count.values())
    ret = emax - emin
    print("Answer for part {} is {}".format(part, ret))


if __name__ == "__main__":
    test_init = "NNCB"
    test_rules = {
        "CH": "B", "HH": "N", "CB": "H", "NH": "C", "HB": "C", "HC": "B",
        "HN": "C", "NN": "C", "BH": "H", "NC": "B", "NB": "B", "BN": "B",
        "BB": "N", "BC": "B", "CC": "N", "CN": "C",
    }
    assert polymerize_step(test_init, test_rules) == "NCNBCHB"
    assert len(polymerize(test_init, test_rules, 10)) == 3073
    assert count_elems_opt(test_init, test_rules, 10) == { 'B': 1749, 'C': 298, 'H': 161, 'N': 865, }

    input_path = "data/input-day14.txt"
    input_init, input_rules = load_input(input_path)
    print_results(input_init, input_rules, 1)
    print_results(input_init, input_rules, 2)

