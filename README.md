# pandocでmarkdownからhtmlを生成してGitHub Pagesで配布するサンプル

## 注意事項

forkするかuse this templateしたら、Settings > Pagesにアクセスし、"Build and deployment"の"Source"を"GitHub Actions"に変更する。そうしないと、GitHub Pagesのデプロイが失敗する。そのためinitial commitによって起動したJobは失敗するが、設定後にJobを再始動させれば成功し、GitHub Pagesとしてアクセスできる。
