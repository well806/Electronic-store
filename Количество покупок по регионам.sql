SELECT c.region, COUNT(s.sale_id) AS number_of_sales 
FROM sales_fact s 
JOIN customers_dim c ON s.customer_id = c.customer_id 
GROUP BY c.region 
ORDER BY number_of_sales DESC;
