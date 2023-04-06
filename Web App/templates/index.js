const uncharged_boats = document.querySelectorAll('#uncharged-berths tr');

uncharged_boats.forEach(boat => {
    input = boat.querySelector('button');
    if (input == undefined)
    {
        return
    }
    input.addEventListener('click', function(event){
        event.preventDefault();
        payed_berth = event.target;
        post_new_berth_payment_status(payed_berth.dataset.berth_id)
        payed_berth.parentNode.parentNode.remove();
    });
});


function post_new_berth_payment_status(berth_id)
{
    body = create_post_request_json_body(berth_id)
    create_update_berth_payment_status_post_request(body);
}


function create_post_request_json_body(berth_id)
{
    return JSON.stringify(
        { 
            "berth_id": parseInt(berth_id),
            "new_berth_payment_status": true
        }
    )
}

function create_update_berth_payment_status_post_request(body)
{
    fetch('/update_berth_payment_status', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: body
    })
}


