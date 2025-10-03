# --- DÁN CODE PYTHON NÀY VÀO MỘT Ô MỚI ---

import pandas as pd
import matplotlib.pyplot as plt
import io

# ▼▼▼ DÁN DỮ LIỆU CỦA THÍ NGHIỆM THỨ NHẤT VÀO ĐÂY ▼▼▼
# Ví dụ: Kết quả của ε-greedy với ε = 0.1
csv_data_1 = """
Step,AverageReward
1,0.16
2,0.32
...
1000,1.42
"""
# ▲▲▲ DÁN DỮ LIỆU CỦA THÍ NGHIỆM THỨ NHẤT VÀO ĐÂY ▲▲▲


# ▼▼▼ DÁN DỮ LIỆU CỦA THÍ NGHIỆM THỨ HAI VÀO ĐÂY ▼▼▼
# Ví dụ: Kết quả của OIV với Q1=5, alpha=0.1
csv_data_2 = """
Step,AverageReward
1,0.05
2,0.12
...
1000,1.48
"""
# ▲▲▲ DÁN DỮ LIỆU CỦA THÍ NGHIỆM THỨ HAI VÀO ĐÂY ▲▲▲


# --- Phần code vẽ đồ thị ---

# Đọc 2 bộ dữ liệu vào 2 bảng (DataFrame) riêng biệt
df1 = pd.read_csv(io.StringIO(csv_data_1))
df2 = pd.read_csv(io.StringIO(csv_data_2))

# Tạo một khung vẽ đồ thị
plt.figure(figsize=(12, 8))

# Vẽ đường cong thứ nhất
plt.plot(df1['Step'], df1['AverageReward'], label='Thí nghiệm 1 (ví dụ: e-greedy, e=0.1)')

# Vẽ đường cong thứ hai trên CÙNG một biểu đồ
plt.plot(df2['Step'], df2['AverageReward'], label='Thí nghiệm 2 (ví dụ: OIV, Q1=5)')

# Thêm các chi tiết cho đồ thị
plt.title('So sánh hiệu suất của các thuật toán', fontsize=16)
plt.xlabel('Steps (Số bước)', fontsize=12)
plt.ylabel('Average Reward (Phần thưởng trung bình)', fontsize=12)
plt.legend() # Hiển thị chú thích cho cả hai đường
plt.grid(True)
plt.show()
