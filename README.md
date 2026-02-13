# Sistema de Supermercado em C#

Este projeto contém uma base de sistema para supermercado em C#, com foco em operações essenciais:

- Cadastro de produtos
- Listagem de produtos
- Reposição de estoque
- Registro de vendas
- Relatório de vendas e faturamento

## Estrutura

- `Program.cs`: interface de linha de comando (menu interativo)
- `Services/SupermercadoService.cs`: regras de negócio
- `Models/`: entidades do domínio (`Produto`, `ItemVenda`, `Venda`)

## Como executar

1. Tenha o .NET SDK (8.0+) instalado.
2. Na pasta do projeto, rode:

```bash
dotnet run
```

## Observações

A aplicação usa armazenamento em memória para simplificar o exemplo. Para uso real, o próximo passo é integrar persistência (ex.: SQL Server) e autenticação de usuários por perfil.
