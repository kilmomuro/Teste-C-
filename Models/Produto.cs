namespace SupermercadoApp.Models;

public class Produto
{
    public required string Codigo { get; init; }
    public required string Nome { get; init; }
    public required decimal Preco { get; private set; }
    public int QuantidadeEstoque { get; private set; }

    public Produto(string codigo, string nome, decimal preco, int quantidadeEstoque)
    {
        Codigo = codigo;
        Nome = nome;
        Preco = preco;
        QuantidadeEstoque = quantidadeEstoque;
    }

    public void AtualizarPreco(decimal novoPreco)
    {
        if (novoPreco <= 0)
        {
            throw new ArgumentException("O preço deve ser maior que zero.");
        }

        Preco = novoPreco;
    }

    public void ReporEstoque(int quantidade)
    {
        if (quantidade <= 0)
        {
            throw new ArgumentException("A quantidade para reposição deve ser positiva.");
        }

        QuantidadeEstoque += quantidade;
    }

    public void RetirarEstoque(int quantidade)
    {
        if (quantidade <= 0)
        {
            throw new ArgumentException("A quantidade para retirada deve ser positiva.");
        }

        if (quantidade > QuantidadeEstoque)
        {
            throw new InvalidOperationException("Estoque insuficiente para a operação.");
        }

        QuantidadeEstoque -= quantidade;
    }
}
