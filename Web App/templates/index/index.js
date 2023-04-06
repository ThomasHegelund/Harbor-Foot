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
        post_new_berth_payment_status(payed_berth.dataset.berth_uuid)
        payed_berth.parentNode.parentNode.remove();
    });
});


function post_new_berth_payment_status(berth_uuid)
{
    body = create_post_request_json_body(berth_uuid)
    create_update_berth_payment_status_post_request(body);
}


function create_post_request_json_body(berth_uuid)
{
    return JSON.stringify(
        { 
            "berth_uuid": berth_uuid,
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


