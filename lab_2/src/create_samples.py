import random


def create_samples(n: int):
    """Создает n пар ключ-значение для тестовых данных"""

    samples_data = [(f'key_{i}', f'value_{i}') for i in range(1, n + 1)]
    return samples_data


def write_samples(data: list[tuple[str, str]], filepath: str):
    """Записывает тестовые данные в файл"""
    try:
        with open(filepath, 'w') as f:
            f.write(str(len(data)) + '\n')
            for key, value in data:
                f.write(f'{key}\n{value}\n')
    except Exception as e:
        print(e)


def main():
    """Генерирует отсортированный, обратный и случайный наборы данных"""
    data = create_samples(13)
    sorted_samples = sorted(data)
    reversed_samples = sorted_samples[::-1]
    random_samples = data.copy()
    random.shuffle(random_samples)

    write_samples(sorted_samples, 'data_samples/sorted_samples.txt')
    write_samples(reversed_samples, 'data_samples/reversed_samples.txt')
    write_samples(random_samples, 'data_samples/random_samples.txt')


if __name__ == "__main__":
    main()
