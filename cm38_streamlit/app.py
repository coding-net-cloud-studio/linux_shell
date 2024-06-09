import streamlit as st

# 创建一个滑块
x = st.slider("选择一个值", 0, 100, 25)

# 根据滑块的值显示文本
st.write(f"你选择的值是 {x}")
