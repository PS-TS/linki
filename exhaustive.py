import itertools
from collections import defaultdict

# 1. Wczytanie danych z pliku
def read_data(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    data = [list(map(int, line.strip().split())) for line in lines if line.strip()]
    return data

# 2. Generowanie macierzy nieodróżnialności
def build_indiscernibility_matrix(data, attributes):
    n = len(data)
    m = len(attributes)
    matrix = [[set() for _ in range(n)] for _ in range(n)]

    for i in range(n):
        for j in range(n):
            if data[i][-1] != data[j][-1]:  # tylko jeśli decyzje różne
                for k in range(m):
                    if data[i][k] == data[j][k]:
                        matrix[i][j].add(attributes[k])
    return matrix

# 3. Sprawdzenie czy kombinacja nie jest sprzeczna
def is_consistent(data, matrix, i, condition):
    for j in range(len(data)):
        if i != j and data[i][-1] != data[j][-1]:
            if all(attr in matrix[i][j] for attr, _ in condition):
                return False
    return True

# 4. Tworzenie reguł dowolnego rzędu z filtrowaniem
def generate_rules(data, attributes, matrix, rzad, forbidden_rules):
    rules = defaultdict(list)
    m = len(attributes)
    n = len(data)

    for i in range(n):
        idxs = range(m)
        for combo in itertools.combinations(idxs, rzad):
            condition = tuple(sorted((attributes[k], data[i][k]) for k in combo))

            # Pomijamy, jeśli zawiera niższy rząd
            skip = False
            for forb in forbidden_rules:
                if set(forb).issubset(set(condition)):
                    skip = True
                    break
            if skip:
                continue

            if is_consistent(data, matrix, i, condition):
                rules[condition].append(i)

    return rules

# 5. Drukowanie reguł
def print_rules(rules, data, rzad):
    print(f"\nREGUŁY RZĘDU {rzad}:")
    for cond, obj_ids in rules.items():
        condition_str = " ∧ ".join([f"({a} = {v})" for a, v in cond])
        decision = data[obj_ids[0]][-1]
        print(f"{condition_str} => (d = {decision}) [{len(obj_ids)}]")

# MAIN
if __name__ == "__main__":
    filename = "values.txt"
    attributes = ['a1', 'a2', 'a3', 'a4', 'a5', 'a6']
    data = read_data(filename)

    matrix = build_indiscernibility_matrix(data, attributes)

    # Reguły rzędu I
    rules_I = generate_rules(data, attributes, matrix, 1, [])
    print_rules(rules_I, data, 1)

    # Reguły rzędu II (pomijamy kombinacje zawierające reguły rzędu I)
    rules_II = generate_rules(data, attributes, matrix, 2, rules_I.keys())
    print_rules(rules_II, data, 2)

    # Reguły rzędu III (pomijamy reguły z I i II)
    all_lower_rules = list(rules_I.keys()) + list(rules_II.keys())
    rules_III = generate_rules(data, attributes, matrix, 3, all_lower_rules)
    print_rules(rules_III, data, 3)
