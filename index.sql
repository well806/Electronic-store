CREATE INDEX idx_sales_fact_product ON sales_fact(product_id);
CREATE INDEX idx_sales_fact_customer ON sales_fact(customer_id);
CREATE INDEX idx_sales_fact_date ON sales_fact(sale_date);
CREATE INDEX idx_products_dim_category ON products_dim(category);
CREATE INDEX idx_customers_dim_region ON customers_dim(region);