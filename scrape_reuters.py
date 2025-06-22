import requests
from bs4 import BeautifulSoup
import json

url = "https://www.reuters.com/world/"
resp = requests.get(url)
soup = BeautifulSoup(resp.content, "html.parser")

headlines = []
for item in soup.select("article h3"):
    title = item.get_text(strip=True)
    link = item.find_parent("a")
    url = link['href'] if link else None
    if title and url:
        headlines.append({"title": title, "url": f"https://www.reuters.com{url}"})

with open("latest_news.json", "w") as f:
    json.dump(headlines, f, indent=2)