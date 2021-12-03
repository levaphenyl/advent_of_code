import pandas as pd


def bits2dec(bitlst):
    bitlst = [ str(b) for b in bitlst ]
    bitstr = "".join(bitlst)
    return int(bitstr, 2)


def gamma_epsilon_rates(bit_mat):
    bit_mat = pd.DataFrame(bit_mat)
    count0 = (bit_mat == 0).sum()
    count1 = (bit_mat == 1).sum()
    gamma = [ 1 if count1[c] > count0[c] else 0 for c in bit_mat.columns ]
    epsilon = [ 0 if i else 1 for i in gamma ]
    gamma = bits2dec(gamma)
    epsilon = bits2dec(epsilon)
    return gamma, epsilon


def load_data(path):
    with open(path, "r") as f:
        rows = f.readlines()

    rows = [ s.strip() for s in rows if len(s) > 1]
    # Convert str to list of int.
    rows = [ [ int(c) for c in s ] for s in rows ]
    return rows


def part1(path):
    bit_mat = load_data(path)
    gamma, epsilon = gamma_epsilon_rates(bit_mat)
    ret = gamma * epsilon
    print("Answer for part 1: {}.".format(ret))


def oxygen_co2_rates(bit_mat):
    def _select_step(selector, c, greater=True):
        count0 = (selector == 0).sum()
        count1 = (selector == 1).sum()
        if greater:
            bit = 0 if count0[c] > count1[c] else 1
        else:
            bit = 1 if count0[c] > count1[c] else 0

        return selector[selector[c] == bit]

    bit_mat = pd.DataFrame(bit_mat)
    oxy_selector = bit_mat
    co2_selector = bit_mat
    for c in bit_mat.columns:
        if len(oxy_selector) > 1:
            oxy_selector = _select_step(oxy_selector, c, True)
        if len(co2_selector) > 1:
            co2_selector = _select_step(co2_selector, c, False)

        if len(oxy_selector) == 1 and len(co2_selector) == 1:
            break

    oxygen = bits2dec(oxy_selector.values[0])
    co2 = bits2dec(co2_selector.values[0])
    return oxygen, co2


def part2(path):
    bit_mat = load_data(path)
    oxy, co2 = oxygen_co2_rates(bit_mat)
    ret = oxy * co2
    print("Answer for part 2: {}.".format(ret))


if __name__ == "__main__":
    test_bits = [ [ 0, 0, 1, 0, 0, ], [ 1, 1, 1, 1, 0, ], [ 1, 0, 1, 1, 0, ],
                  [ 1, 0, 1, 1, 1, ], [ 1, 0, 1, 0, 1, ], [ 0, 1, 1, 1, 1, ],
                  [ 0, 0, 1, 1, 1, ], [ 1, 1, 1, 0, 0, ], [ 1, 0, 0, 0, 0, ],
                  [ 1, 1, 0, 0, 1, ], [ 0, 0, 0, 1, 0, ], [ 0, 1, 0, 1, 0, ],
    ]
    assert gamma_epsilon_rates(test_bits) == (22, 9)
    assert oxygen_co2_rates(test_bits) == (23, 10)

    input_path = "data/input-day3.txt"
    part1(input_path)
    part2(input_path)

