SELECT p.category, SUM(s.amount) AS total_sales 
FROM sales_fact s 
JOIN products_dim p ON s.product_id = p.product_id 
GROUP BY p.category 
ORDER BY total_sales DESC;
