import pandas as pd
import matplotlib.pyplot as plt
import io

# ▼▼▼ DÁN DỮ LIỆU CSV CỦA BẠN VÀO ĐÂY ▼▼▼
csv_data = """
Step,AverageReward,OptimalActionPercentage
1,0.25,10.5
2,0.45,15.2
...
1000,1.48,85.5
"""
# ▲▲▲ DÁN DỮ LIỆU CSV CỦA BẠN VÀO ĐÂY ▲▲▲


# --- Phần code vẽ 2 biểu đồ ---

# Đọc dữ liệu CSV từ văn bản
df = pd.read_csv(io.StringIO(csv_data))

# Tạo một khung chứa 2 biểu đồ, xếp chồng lên nhau (2 hàng, 1 cột)
fig, axs = plt.subplots(2, 1, figsize=(12, 10))

# --- Biểu đồ 1: Phần thưởng Trung bình ---
axs[0].plot(df['Step'], df['AverageReward'], color='blue')
axs[0].set_title('Phan thuong Trung binh theo Thoi gian (Average Reward vs. Steps)', fontsize=14)
axs[0].set_ylabel('Average Reward')
axs[0].grid(True)

# --- Biểu đồ 2: Tỷ lệ Hành động Tối ưu ---
axs[1].plot(df['Step'], df['OptimalActionPercentage'], color='green')
axs[1].set_title('Ty le Chon hanh dong Toi uu (Optimal Action % vs. Steps)', fontsize=14)
axs[1].set_xlabel('Steps (So buoc)')
axs[1].set_ylabel('% Optimal Action')
# Định dạng trục Y để hiển thị ký hiệu %
axs[1].yaxis.set_major_formatter(plt.FuncFormatter('{:.0f}%'.format))
axs[1].grid(True)


# Tự động điều chỉnh khoảng cách giữa các biểu đồ
plt.tight_layout()

# Hiển thị kết quả
plt.show()
