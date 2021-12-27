import numpy as np


def decode_packet(bitstr):
    ret = {}
    ret["version"] = int(bitstr[:3], 2)
    ret["type"] = int(bitstr[3:6], 2)
    if ret["type"] == 4:
        # Literal packet
        i = 6
        literal = ''
        while bitstr[i] == '1':
            literal += bitstr[i+1:i+5]
            i += 5

        literal += bitstr[i+1:i+5]
        ret["literal"] = int(literal, 2)
        ret["length"] = i + 5
    else:
        # Operator packet
        lentype = bitstr[6]
        values = []
        if lentype == '1':
            i = 18
            n_subpck = int(bitstr[7:i], 2)
            for _ in range(n_subpck):
                subpck = bitstr[i:]
                subpck = decode_packet(subpck)
                values.append(subpck)
                i += subpck["length"]

        else:
            subpck_len = int(bitstr[7:22], 2)
            i = 22
            while i < 22 + subpck_len:
                subpck = bitstr[i:]
                subpck = decode_packet(subpck)
                values.append(subpck)
                i += subpck["length"]

        ret["operators"] = values
        ret["length"] = i

    return ret


def hex2bits(hexstr):
    bitstr = int.from_bytes(bytes.fromhex(hexstr), "big")
    fmt = '0{}b'.format(4 * len(hexstr)) # Ensure the padding with zeros.
    return format(bitstr, fmt)


def sum_versions(packet):
    ret = packet["version"]
    if "operators" in packet:
        ret += sum([ sum_versions(p) for p in packet["operators"] ])

    return ret


def compute(packet):
    if "operators" in packet:
        operands = [ compute(p) for p in packet["operators"] ]
        if packet["type"] == 0:
            return sum(operands)
        elif packet["type"] == 1:
            return np.product(operands)
        elif packet["type"] == 2:
            return min(operands)
        elif packet["type"] == 3:
            return max(operands)
        elif packet["type"] == 5:
            return int(operands[0] > operands[1])
        elif packet["type"] == 6:
            return int(operands[0] < operands[1])
        elif packet["type"] == 7:
            return int(operands[0] == operands[1])
    else:
        return packet["literal"]


def load_input(path):
    with open(path, 'r') as f:
        rows = f.readlines()

    rows = [ r.strip() for r in rows if len(r) > 1 ]
    msg = rows[0]
    return msg


def print_results(hexstr, part):
    bitstr = hex2bits(hexstr)
    packet = decode_packet(bitstr)
    if part == 1:
        ret = sum_versions(packet)
    elif part == 2:
        ret = compute(packet)
    else:
        raise NotImplementedError()

    print("Answer for part {} is {}".format(part, ret))


if __name__ == "__main__":
    test_input = [
        "D2FE28",
        "38006F45291200",
        "EE00D40C823060",
        "8A004A801A8002F478",
        "9C0141080250320F1802104A08",
    ]
    assert hex2bits(test_input[0]) == "110100101111111000101000"
    assert decode_packet(hex2bits(test_input[0])) == { "version": 6, "type": 4, "literal": 2021, "length": 21, }
    test1 = decode_packet(hex2bits(test_input[1]))
    assert test1["version"] == 1
    assert test1["type"] == 6
    assert [ o["literal"] for o in test1["operators"] ] == [ 10, 20, ]
    test2 = decode_packet(hex2bits(test_input[2]))
    assert test2["version"] == 7
    assert test2["type"] == 3
    assert [ o["literal"] for o in test2["operators"] ] == [ 1, 2, 3, ]
    test3 = decode_packet(hex2bits(test_input[3]))
    assert sum_versions(test3) == 16
    test4 = decode_packet(hex2bits(test_input[4]))
    assert compute(test4) == 1

    input_path = "data/input-day16.txt"
    input_msg = load_input(input_path)
    print_results(input_msg, 1)
    print_results(input_msg, 2)

