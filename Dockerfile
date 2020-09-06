FROM bretfisher/jekyll-serve:latest

WORKDIR /usr/src/app

COPY Gemfile ./
RUN bundle install

COPY . .