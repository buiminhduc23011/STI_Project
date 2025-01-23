# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import os

# Đường dẫn thư mục xuất ảnh
output_dir = "pie_charts"
os.makedirs(output_dir, exist_ok=True)

# Màu sắc
primary_color = "#1890FF"  # Màu chính
secondary_color = "#EFEFEF"  # Màu phụ

# Kích thước hình ảnh cần tạo
desired_width = 100  # px
desired_height = 100  # px
dpi = 10000  # Độ phân giải

# Tính kích thước figsize bằng inch
figsize = (desired_width / dpi, desired_height / dpi)

# Tạo và lưu biểu đồ
for percentage in range(101):  # Từ 0% đến 100%
    # Dữ liệu biểu đồ
    sizes = [percentage, 100 - percentage]
    colors = [primary_color, secondary_color]

    # Tạo biểu đồ
    fig, ax = plt.subplots(figsize=figsize, dpi=dpi)  # Đảm bảo đúng kích thước
    ax.pie(
        sizes,
        colors=colors,
        startangle=90,
        counterclock=False,
        wedgeprops=dict(width=0.3)  # Độ rộng của pie chart
    )
    # Xóa viền và nền
    ax.set_aspect('equal')
    ax.axis('off')

    # Lưu ảnh mà không có bất kỳ padding hoặc không gian dư thừa nào
    file_path = os.path.join(output_dir, f"{percentage}.png")
    plt.savefig(file_path, transparent=True, bbox_inches='tight', pad_inches=0)
    plt.close(fig)

print(f"Tất cả biểu đồ đã được lưu trong thư mục: {output_dir}")
