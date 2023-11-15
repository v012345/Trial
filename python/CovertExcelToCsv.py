try:
    import pandas as pd
except ImportError:
    print("pandas is not installed. Installing...")
    try:
        import subprocess

        subprocess.check_call(["pip", "install", "pandas"])
        print("pandas installed successfully.")
        import pandas as pd  # 重新尝试导入
    except Exception as e:
        print(f"Error installing pandas: {e}")
        exit(1)
try:
    import xlrd
except ImportError:
    print("xlrd is not installed. Installing...")
    try:
        import subprocess

        subprocess.check_call(["pip", "install", "xlrd"])
        print("xlrd installed successfully.")
    except Exception as e:
        print(f"Error installing xlrd: {e}")
        exit(1)
import argparse

print("Conversion complete.")


def convert_excel_to_csv(input_path, output_path):
    df = pd.read_excel(input_path)
    df.to_csv(output_path, index=False, encoding="utf-8")


def main():
    # 创建 ArgumentParser 对象
    parser = argparse.ArgumentParser(description="Convert Excel to CSV")

    # 添加命令行参数
    parser.add_argument(
        "-I",
        "--input",
        dest="input_path",
        required=True,
        help="Input path to Excel file",
    )
    parser.add_argument(
        "-O",
        "--output",
        dest="output_path",
        required=True,
        help="Output path for CSV file",
    )

    # 解析命令行参数
    args = parser.parse_args()

    # 执行转换操作
    convert_excel_to_csv(args.input_path, args.output_path)

if __name__ == "__main__":
    main()
