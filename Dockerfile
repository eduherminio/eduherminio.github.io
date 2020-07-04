FROM bretfisher/jekyll-serve as base

WORKDIR /usr/src/app

COPY Gemfile ./
RUN bundle install

FROM base as final

COPY . .