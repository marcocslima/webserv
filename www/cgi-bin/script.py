import os

def main():
    for var, value in os.environ.items():
        print(f'{var} = {value}')   

if __name__ == '__main__':
    main()