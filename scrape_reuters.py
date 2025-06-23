import feedparser
import os
from datetime import datetime
import pytz

# List of trusted RSS feeds
NEWS_FEEDS = {
    "Reuters": "http://feeds.reuters.com/reuters/topNews",
    "Associated Press": "https://apnews.com/hub/ap-top-news/rss.xml",
    "BBC News": "http://feeds.bbci.co.uk/news/world/rss.xml",
    "Al Jazeera": "https://www.aljazeera.com/xml/rss/all.xml",
    "The Guardian": "https://www.theguardian.com/world/rss",
}

def fetch_news():
    """Fetches news from the RSS feeds and formats it."""
    all_news = []
    for source, url in NEWS_FEEDS.items():
        try:
            feed = feedparser.parse(url)
            for entry in feed.entries[:10]:  # Get latest 10 articles from each source
                # Get publication date and convert to a standard format
                published_time = datetime.now(pytz.utc) # Default to now
                if hasattr(entry, 'published_parsed') and entry.published_parsed:
                    published_time = datetime(*entry.published_parsed[:6], tzinfo=pytz.utc)

                all_news.append({
                    "source": source,
                    "title": entry.title,
                    "link": entry.link,
                    "published": published_time,
                    "summary": entry.get("summary", "No summary available.")
                })
        except Exception as e:
            print(f"Could not fetch news from {source}: {e}")

    # Sort all news by publication date, newest first
    all_news.sort(key=lambda x: x["published"], reverse=True)
    return all_news

def generate_markdown(news_items):
    """Generates a Markdown formatted string from news items."""
    # Get current time in Tehran for the header
    tehran_tz = pytz.timezone("Asia/Tehran")
    now_tehran = datetime.now(tehran_tz).strftime("%Y-%m-%d %H:%M:%S %Z")

    markdown = f"# Uncensored News Update\n\n_Last updated: {now_tehran}_\n\n"

    for item in news_items:
        # Format the publication date to Tehran time
        published_tehran = item['published'].astimezone(tehran_tz).strftime("%Y-%m-%d %H:%M")
        markdown += f"## {item['title']}\n"
        markdown += f"**Source:** {item['source']}  \n"
        markdown += f"**Published:** {published_tehran} (Tehran Time)  \n"
        markdown += f"**Link:** [{item['link']}]({item['link']})  \n\n"
        markdown += f"{item['summary']}\n\n"
        markdown += "---\n\n"

    return markdown

if __name__ == "__main__":
    news = fetch_news()
    markdown_content = generate_markdown(news)
    with open("NEWS.md", "w", encoding="utf-8") as f:
        f.write(markdown_content)
    print("Successfully generated NEWS.md")